

#include "./LogWriter.hpp"

#include <atomic>
#include <cerrno>
#include <system_error>

LogWriter::~LogWriter() = default;

void LogWriterDummy::write_error(const std::string &) {
  // noop
}

LogWriterDummy::~LogWriterDummy() {
  // noop
}

static char global_file_buf[1 << 20]; // 1 MiB buffer
std::atomic_flag using_file_buf = false;

void LogWriterFile::initialize_file(const std::string &file) {
  FILE *file_ptr = fopen(file.c_str(), "a");
  if (file_ptr == NULL) {
    throw std::system_error{errno, std::generic_category()};
  }
  bool was_set = using_file_buf.test_and_set(std::memory_order_acquire);

  if (was_set) {
    // makes writes faster, but as global_file_buf is used for all
    // implementations, this can only be used, if no other instance is used, so
    // guarding it with an atomic flag
    int result = setvbuf(this->m_file, global_file_buf, _IOFBF,
                         sizeof(global_file_buf) / sizeof(*global_file_buf));

    if (result != 0) {
      throw std::system_error{errno, std::generic_category()};
    }
  }
}

LogWriterFile::LogWriterFile(const std::string &file) { initialize_file(file); }

void LogWriterFile::write_error(const std::string &error) {
  fprintf(this->m_file, "%s\n", error.c_str());
}

LogWriterFile::~LogWriterFile() {

  fflush(this->m_file);
  fclose(this->m_file);
}

LogWriterFileMultithreaded::LogWriterFileMultithreaded(const std::string &file)
    : m_lock{}, m_impl{LogWriterFile{file}} {}

void LogWriterFileMultithreaded::write_error(const std::string &error) {
  std::lock_guard<std::mutex> lock(this->m_lock);

  this->m_impl.write_error(error);
}

LogWriterFileMultithreaded::~LogWriterFileMultithreaded() = default;
