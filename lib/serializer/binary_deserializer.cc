//===------------------------------------------===
// estd 2023
//
// Identification: lib/serializer/binary_deserializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "serializer/binary_deserializer.hh"

namespace estd {

void BinaryDeserializer::SetTag(const char *tag) {
  currentTag = tag;
  stack.back().readFieldCount++;
}

//===--------------------------------------------------------------------===//
// complex types
//===--------------------------------------------------------------------===//

void BinaryDeserializer::OnObjectBegin() {
  auto expected_field_count = ReadPOD<uint32_t>();
  auto expected_size = ReadPOD<uint64_t>();
  stack.emplace_back(expected_field_count, expected_size);
}

void BinaryDeserializer::OnObjectEnd() {
  auto &frame = stack.back();
  if (frame.readFieldCount < frame.expectedFieldCount) {
    throw ParseException("Not all fields were read.");
  }
  stack.pop_back();
}

auto BinaryDeserializer::OnVectorBegin() -> size_t { return ReadPOD<size_t>(); }

auto BinaryDeserializer::OnMapBegin() -> size_t { return ReadPOD<size_t>(); }

//===------------------------------------------------------------------------===
// Primitive types
//===------------------------------------------------------------------------===

auto BinaryDeserializer::ReadBool() -> bool { return ReadPOD<bool>(); }

auto BinaryDeserializer::ReadSignedInt8() -> int8_t {
  return ReadPOD<int8_t>();
}

auto BinaryDeserializer::ReadUnsignedInt8() -> uint8_t {
  return ReadPOD<uint8_t>();
}

auto BinaryDeserializer::ReadSignedInt16() -> int16_t {
  return ReadPOD<int16_t>();
}

auto BinaryDeserializer::ReadUnsignedInt16() -> uint16_t {
  return ReadPOD<uint16_t>();
}

auto BinaryDeserializer::ReadSignedInt32() -> int32_t {
  return ReadPOD<int32_t>();
}

auto BinaryDeserializer::ReadUnsignedInt32() -> uint32_t {
  return ReadPOD<uint32_t>();
}

auto BinaryDeserializer::ReadSignedInt64() -> int64_t {
  return ReadPOD<int64_t>();
}

auto BinaryDeserializer::ReadUnsignedInt64() -> uint64_t {
  return ReadPOD<uint64_t>();
}

auto BinaryDeserializer::ReadFloat() -> float { return ReadPOD<float>(); }

auto BinaryDeserializer::ReadDouble() -> double { return ReadPOD<double>(); }

auto BinaryDeserializer::ReadString() -> string {
  auto length = ReadPOD<std::size_t>();
  if (length == 0) {
    return "";
  }
  auto buffer = vector<char>();
  buffer.reserve(length);
  ReadData(buffer, length);
  return {buffer.begin(), buffer.end()};
}

} // namespace estd
