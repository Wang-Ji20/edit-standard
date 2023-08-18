//===------------------------------------------===
// estd 2023
//
// Identification: include/hash.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_HASH_HH
#define ESTD_HASH_HH

#include <cstddef>
#include <cstdint>

namespace estd {

struct HashingConstants {
  static constexpr uint64_t kMagicOne = 0x9e3779b97f4a7c15ULL;
  static constexpr uint64_t kMagicTwo = 0x9e3779b97f4a7c15ULL;
};

/// This struct provides some hashing utilities.
/// for example checksum, hash, etc.
struct HashingUtilities {

  static auto Checksum(const uint64_t data) -> uint64_t {
    return data * HashingConstants::kMagicOne;
  }

  static auto Checksum(const uint8_t *data, size_t size) -> uint64_t {
    uint64_t inEightBytes = size / sizeof(uint64_t);
    auto *data64 = reinterpret_cast<const uint64_t *>(data);
    uint64_t checksum = 0;
    for (auto i = 0ULL; i < inEightBytes; ++i) {
      checksum ^= Checksum(data64[i]);
    }
    for (auto i = inEightBytes * sizeof(uint64_t); i < size; ++i) {
      checksum ^= Checksum(data[i]);
    }
    return checksum;
  }

};

} // namespace estd

#endif
