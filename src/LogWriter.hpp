

#pragma once

#include <mutex>
#include <string>

struct LogWriter {
  virtual void write_error(const std::string &error) = 0;

  virtual ~LogWriter() = 0;
};

struct LogWriterDummy final : LogWriter {
  void write_error(const std::string &error) override;

  ~LogWriterDummy() override;
};

struct LogWriterFile final : LogWriter {

private:
  // use c files,for fast performance
  FILE *m_file;

  void initialize_file(const std::string &file);

public:
  explicit LogWriterFile(const std::string &file);

  void write_error(const std::string &error) override;

  LogWriterFile(LogWriterFile &&other);
  LogWriterFile(const LogWriterFile &) = delete;

  LogWriterFile &operator=(LogWriterFile &&other);
  LogWriterFile &operator=(const LogWriterFile &) = delete;

  ~LogWriterFile() override;
};

struct LogWriterFileMultithreaded final : LogWriter {

private:
  std::mutex m_lock;
  // use c files,for fast performance
  LogWriterFile m_impl;

public:
  explicit LogWriterFileMultithreaded(const std::string &file);

  void write_error(const std::string &error) override;

  ~LogWriterFileMultithreaded() override;
};
