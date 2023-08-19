//===------------------------------------------===
// estd 2023
//
// Identification: include/serializer/binary_serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "serializer/serializer.hh"
#include <vector>

namespace estd {

using std::vector;

/// Serialize format:
/// 1. For primitive types, we use the binary format of the type.
/// 2. For object types, we use the following format:
///    1. The first 4 bytes is the field count.
///    2. The next 8 bytes is the size of the object.
/// 3. for STL container types, we use the following format:
///    1. The first 4 bytes is the container size
///    2. The next are elements in the container
class BinarySerializer : public Serializer {
public:
  auto
  Serialize() -> std::vector<uint8_t> override {
    return {data.begin(), data.end()};
  }

  void
  Clear() override {
    data.clear();
    stack.clear();
    stack.emplace_back(0, 0, 0);
  }

private:
  //===------------------------------------------------------------------------===
  // Internal Implementation
  //===------------------------------------------------------------------------===

  struct State {
    uint32_t fieldCount;
    size_t objectSize;
    size_t bufferOffset;
    State(uint32_t fieldCount, size_t objectSize, size_t bufferOffset)
        : fieldCount(fieldCount),
          objectSize(objectSize),
          bufferOffset(bufferOffset) {}
  };

  const char *current_tag;

  vector<char> data;

  vector<State> stack;

  template <typename T>
  void
  Store(const T &value, char *buffer) {
    std::memcpy(buffer, (void *)&value, sizeof(T));
  }

  template <typename T>
  void
  Write(T value) {
    static_assert(std::is_trivially_destructible<T>::value,
                  "T must be trivally destructible-typed variable");
    WriteData(reinterpret_cast<const char *>(&value), sizeof(T));
  }

  void
  WriteData(const char *buffer, size_t size) {
    data.insert(data.end(), buffer, buffer + size);
    stack.back().objectSize += size;
  }

  explicit BinarySerializer() { stack.emplace_back(0, 0, 0); };

  //===------------------------------------------------------------------------===
  // parent interface
  //===------------------------------------------------------------------------===
public:
  template <typename T>
  static auto
  Serialize(T &obj) -> vector<char> {
    BinarySerializer serializer;
    serializer.WriteValue(obj);
    return std::move(serializer.data);
  }

  template <typename T>
  static auto
  Serialize(const T &obj, char *buffer, size_t bufferSize) -> size_t {
    BinarySerializer serializer;
    serializer.WriteValue(obj);
    auto size = serializer.data.size();
    if (size > bufferSize) {
      throw std::runtime_error("Failed to serialize: buffer size is too small");
    }
    std::memcpy(buffer, serializer.data.data(), size);
    return size;
  }

  void
  SetTag(const char *tag) override;

  void
  OnObjectBegin() final;
  void
  OnObjectEnd() final;

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  void
  OnVectorBegin(size_t size) final;

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  void
  OnUnorderedMapBegin(size_t size) final;

  // https://stackoverflow.com/questions/3678197/virtual-function-implemented-in-base-class-not-being-found-by-compiler
  using Serializer::WriteValue;
  void
  WriteValue(const char *value) final;
  void
  WriteNull() final;
  void
  WriteValue(bool value) final;
  void
  WriteValue(uint8_t value) final;
  void
  WriteValue(int8_t value) final;
  void
  WriteValue(uint16_t value) final;
  void
  WriteValue(int16_t value) final;
  void
  WriteValue(uint32_t value) final;
  void
  WriteValue(int32_t value) final;
  void
  WriteValue(uint64_t value) final;
  void
  WriteValue(int64_t value) final;
  void
  WriteValue(float value) final;
  void
  WriteValue(double value) final;
};

} // namespace estd
