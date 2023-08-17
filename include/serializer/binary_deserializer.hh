//===------------------------------------------===
// estd 2023
//
// Identification: include/serializer/binary_deserializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "serializer/deserializer.hh"

namespace estd {

class BinaryDeserializer : public Deserializer {
public:
  template <typename T>
  static auto Deserialize(char *ptr, size_t length) -> unique_ptr<T> {
    BinaryDeserializer deserializer(ptr, length);
    auto result = deserializer.Read<T>();
    return make_unique<T>(std::move(result));
  }

private:
  explicit BinaryDeserializer(char *ptr, size_t length)
      : ptr_(ptr), endPtr_(ptr + length) {}

  struct State {
    uint32_t expectedFieldCount;
    size_t expectedSize;
    uint32_t readFieldCount = 0;
    State(uint32_t expected_field_count, size_t expected_size)
        : expectedFieldCount(expected_field_count),
          expectedSize(expected_size) {}
  };

  const char *currentTag = nullptr;
  char *ptr_;
  char *endPtr_;
  vector<State> stack;

  template <typename T> auto ReadPOD() -> T {
    static_assert(std::is_trivially_destructible<T>::value,
                  "T must be trivally destructible-typed variable");
    T value;
    ReadData((char *)&value, sizeof(T));
    return value;
  }

  void ReadData(char *buffer, size_t read_size) {
    if (ptr_ + read_size > endPtr_) {
      throw ParseException("Failed to deserialize: not enough data in buffer "
                           "to fulfill read request");
    }
    memcpy(buffer, ptr_, read_size);
    ptr_ += read_size;
  }

  void ReadData(vector<char> &data, size_t read_size) {
    if (ptr_ + read_size > endPtr_) {
      throw ParseException("Failed to deserialize: not enough data in buffer "
                           "to fulfill read request");
    }
    data.insert(data.end(), ptr_, ptr_ + read_size);
    ptr_ += read_size;
  }

public:
  //===------------------------------------------------------------------------===
  // override functions
  //===------------------------------------------------------------------------===
  void SetTag(const char *tag) override;
  auto OnVectorBegin() -> size_t override;
  auto OnMapBegin() -> size_t override;
  void OnObjectBegin() override;
  void OnObjectEnd() override;

protected:
  auto ReadBool() -> bool override;
  auto ReadSignedInt8() -> int8_t override;
  auto ReadUnsignedInt8() -> uint8_t override;
  auto ReadSignedInt16() -> int16_t override;
  auto ReadUnsignedInt16() -> uint16_t override;
  auto ReadSignedInt32() -> int32_t override;
  auto ReadUnsignedInt32() -> uint32_t override;
  auto ReadSignedInt64() -> int64_t override;
  auto ReadUnsignedInt64() -> uint64_t override;
  auto ReadFloat() -> float override;
  auto ReadDouble() -> double override;
  auto ReadString() -> string override;
};

} // namespace estd
