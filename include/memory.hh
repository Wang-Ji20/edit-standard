//===------------------------------------------===
// estd 2023
//
// Identification: include/memory.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_MEMORY_HH
#define ESTD_MEMORY_HH

#include <cstddef>
#include <cstdint>

namespace estd {

struct MemoryUtils {

  template <typename T>
  static auto
  Store(uint8_t *ptr, const T &value) -> void {
    T *p = reinterpret_cast<T *>(ptr);
    *p = value;
  }

  template <typename T>
  static auto
  Load(const uint8_t *ptr) -> T {
    return Load<T>(ptr, 0ULL);
  }

  template <typename T>
  static auto
  Load(const uint8_t *ptr, size_t offset) -> T {
    return *reinterpret_cast<const T *>(ptr + offset);
  }
};

} // namespace estd

#endif
