
#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include <csv/datasource/IDataSource.hpp>
#include <csv/datasource/utf8/DataSource.hpp>

namespace source {
class MemoryMappedDataSource : public csv::utf8::DataSource {
public:
  MemoryMappedDataSource() noexcept {}
  ~MemoryMappedDataSource();

  /// Throws csv::file_exception if unable to open file
  MemoryMappedDataSource(const std::filesystem::path &file, off_t offset,
                         std::size_t length) {
    if (!open(file, offset, length)) {
      throw csv::file_exception();
    }
  }
  bool open(const std::filesystem::path &file, off_t offset,
            std::size_t length);
  void close();

public:
  virtual bool next();
  virtual void back();
  virtual double progress();

private:
  int _fd;
  std::size_t _length{};

  std::uint8_t *_data{nullptr};
  std::size_t _current_pos{};
};

} // namespace source
