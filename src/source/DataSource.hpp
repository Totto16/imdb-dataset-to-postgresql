
#pragma once

#include <filesystem>
#include <stdexcept>

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
    if (const auto value = open(file, offset, length); value.has_value()) {
      throw std::runtime_error(value.value());
    }
  }
  std::optional<std::string> open(const std::filesystem::path &file,
                                  off_t offset, std::size_t length);
  void close();

public:
  bool next() override;
  void back() override;
  double progress() override;

  // we don't support quotes
  inline bool is_quote() const override { return false; }

private:
  int _fd{};
  std::size_t _length{};

  std::uint8_t *_data{nullptr};
  std::size_t _current_pos{};
};

} // namespace source
