
#include <array>
#include <cstdint>
#include <fcntl.h>
#include <filesystem>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "DataSource.hpp"

namespace {
// BOM definitions
static const constexpr size_t _BOMS_SIZE = 3;
static const std::array<std::uint8_t, _BOMS_SIZE> _BOMS = {0xEF, 0xBB, 0xBF};

} // namespace
source::MemoryMappedDataSource::~MemoryMappedDataSource() { close(); }

void source::MemoryMappedDataSource::close() {
  if (_data == nullptr) {
    munmap(_data, _length);
    ::close(_fd);
    _data = nullptr;
    _length = 0;
    _current_pos = 0;
  }
}

bool source::MemoryMappedDataSource::open(const std::filesystem::path &file,
                                          off_t offset, std::size_t length) {

  // If we have one open, close it first
  close();

  _length = length;
  _current_pos = 0;

  if (!std::filesystem::exists(file)) {
    return false;
  }

  _fd = ::open(file.string().c_str(), O_RDONLY, 0);

  if (_fd < 0) {
    return false;
  }

  off_t final_offset = offset;

  long page_size = sysconf(_SC_PAGE_SIZE);
  if (page_size < 0) {
    return false;
  }

  if (offset % page_size != 0) {
    long difference = offset % page_size;
    _length += difference;
    final_offset -= difference;
    _current_pos = difference;
  }

  this->_data = static_cast<std::uint8_t *>(mmap(nullptr, _length, PROT_READ,
                                                 MAP_PRIVATE | MAP_POPULATE,
                                                 _fd, final_offset));

  if (_data == MAP_FAILED) {
    return false;
  }

  if (_length < _BOMS_SIZE) {
    // If we have less chars in the file than the size of the BOM.
    return true;
  }

  // Check if we have a BOM - Read the first three bytes.
  static_assert(_BOMS_SIZE == 3);
  std::array<std::uint8_t, _BOMS_SIZE> utf8BOM = {
      _data[_current_pos], _data[_current_pos + 1], _data[_current_pos + 2]};

  if (memcmp(_BOMS.data(), utf8BOM.data(), _BOMS_SIZE) == 0) {
    _current_pos += _BOMS_SIZE; // skip the bom bytes!
  }

  return true;
}

double source::MemoryMappedDataSource::progress() {
  double pos = _current_pos;
  double len = _length;
  return std::min(pos / len, 1.0);
}

bool source::MemoryMappedDataSource::next() {
  if (_current_pos >= _length) {
    return false;
  }

  _prev = _current;
  _current = _data[_current_pos];
  _current_pos++;
  return true;
}

void source::MemoryMappedDataSource::back() {
  _current = _prev;
  _prev = 0;
  _current_pos--;
}
