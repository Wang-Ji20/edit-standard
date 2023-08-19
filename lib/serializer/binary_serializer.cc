//===------------------------------------------===
// estd 2023
//
// Identification: lib/serializer/binary_serializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "serializer/binary_serializer.hh"

namespace estd {

void
BinarySerializer::SetTag(const char *tag) {
  current_tag = tag;
  stack.back().fieldCount++;
}

//===------------------------------------------------------------------------===
// complex types
//===------------------------------------------------------------------------===

void
BinarySerializer::OnVectorBegin(size_t size) {
  Write<size_t>(size);
}

void
BinarySerializer::OnUnorderedMapBegin(size_t size) {
  Write<size_t>(size);
}

void
BinarySerializer::OnObjectBegin() {
  stack.emplace_back(0, 0ULL, data.size());
  Write<uint32_t>(0);  // Placeholder for the field count
  Write<size_t>(0ULL); // Placeholder for the size
}

void
BinarySerializer::OnObjectEnd() {
  auto &frame = stack.back();
  // Patch the field count and size
  auto *message_start = &data[frame.bufferOffset];
  Store<uint32_t>(frame.fieldCount, message_start);
  Store<size_t>(frame.objectSize, message_start + sizeof(uint32_t));
  stack.pop_back();
}

//===------------------------------------------------------------------------===
// primitive types
//===------------------------------------------------------------------------===

void
BinarySerializer::WriteNull() {}

void
BinarySerializer::WriteValue(bool value) {
  Write<bool>(value);
}

void
BinarySerializer::WriteValue(uint8_t value) {
  Write<uint8_t>(value);
}

void
BinarySerializer::WriteValue(uint16_t value) {
  Write<uint16_t>(value);
}

void
BinarySerializer::WriteValue(uint32_t value) {
  Write<uint32_t>(value);
}

void
BinarySerializer::WriteValue(uint64_t value) {
  Write<uint64_t>(value);
}

void
BinarySerializer::WriteValue(int8_t value) {
  Write<int8_t>(value);
}

void
BinarySerializer::WriteValue(int16_t value) {
  Write<int16_t>(value);
}

void
BinarySerializer::WriteValue(int32_t value) {
  Write<int32_t>(value);
}

void
BinarySerializer::WriteValue(int64_t value) {
  Write<int64_t>(value);
}

void
BinarySerializer::WriteValue(float value) {
  Write<float>(value);
}

void
BinarySerializer::WriteValue(double value) {
  Write<double>(value);
}

void
BinarySerializer::WriteValue(const char *value) {
  auto length = strlen(value);
  Write<size_t>(length);
  WriteData(value, length);
}

} // namespace estd
