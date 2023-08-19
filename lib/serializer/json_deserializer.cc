//===------------------------------------------===
// estd 2023
//
// Identification: lib/serializer/json_deserializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "serializer/json_deserializer.hh"

namespace estd {

auto
JsonDeserializer::GetNextValue() -> yyjson_val * {
  auto &current = GetCurrent();
  // current is either an array or an object
  yyjson_val *val = nullptr;
  if (yyjson_is_obj(current.val)) {
    val = yyjson_obj_get(current.val, currentTag);
    if (val == nullptr) {
      const char *json = yyjson_val_write(GetCurrent().val, 0, nullptr);
      string currentObj{json};
      free((void *)json);
      throw ParseException("Cannot find property: " + string(currentTag) +
                           " in object: " + currentObj);
    }
    return val;
  }

  if (!yyjson_is_arr(current.val)) {
    throw ParseException(
        "Cannot get nested value from non object or array-type");
  }

  // current is an array
  val = yyjson_arr_iter_next(&current.arr_iter);
  if (val == nullptr) {
    const char *json = yyjson_val_write(GetCurrent().val, 0, nullptr);
    string currentArr{json};
    free((void *)json);
    throw ParseException("Cannot find next element in array: " + currentArr);
  }
  return val;
}

void
JsonDeserializer::DumpDoc() {
  const char *json = yyjson_write(doc, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

void
JsonDeserializer::DumpCurrent() {
  const char *json = yyjson_val_write(GetCurrent().val, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

void
JsonDeserializer::Dump(yyjson_mut_val *val) {
  const char *json = yyjson_mut_val_write(val, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

void
JsonDeserializer::Dump(yyjson_val *val) {
  const char *json = yyjson_val_write(val, 0, nullptr);
  fprintf(stderr, "%s\n", json);
  free((void *)json);
}

[[noreturn]] void
JsonDeserializer::ThrowTypeError(yyjson_val *val, const char *expected) {
  const auto *actual = yyjson_get_type_desc(val);
  auto &parent = GetCurrent();
  if (yyjson_is_obj(parent.val)) {
    throw ParseException("Object expect child of type: " + string(expected) +
                         ", but got type: " + string(actual));
  }
  if (yyjson_is_arr(parent.val)) {
    throw ParseException("Array expect child of type: " + string(expected) +
                         ", but got type: " + string(actual));
  }
  throw std::runtime_error("Cannot throw type error for non-object or array");
}

//===------------------------------------------------------------------------===
// override hooks
//===------------------------------------------------------------------------===

auto
JsonDeserializer::OnVectorBegin() -> size_t {
  auto *val = GetNextValue();
  if (!yyjson_is_arr(val)) {
    ThrowTypeError(val, "array");
  }
  Push(val);
  return yyjson_arr_size(val);
}

void
JsonDeserializer::OnVectorEnd() {
  Pop();
}

auto
JsonDeserializer::OnMapBegin() -> size_t {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "array");
  }
  Push(val);
  return yyjson_obj_size(val);
}

void
JsonDeserializer::OnMapEnd() {
  Pop();
}

void
JsonDeserializer::OnMapEntryBegin() {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "object");
  }
  Push(val);
}

void
JsonDeserializer::OnMapEntryEnd() {
  Pop();
}

void
JsonDeserializer::OnMapKeyBegin() {
  SetTag("key");
}

void
JsonDeserializer::OnMapValueBegin() {
  SetTag("value");
}

void
JsonDeserializer::OnObjectBegin() {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "object");
  }
  Push(val);
}

void
JsonDeserializer::OnObjectEnd() {
  Pop();
}

void
JsonDeserializer::OnPairBegin() {
  auto *val = GetNextValue();
  if (!yyjson_is_obj(val)) {
    ThrowTypeError(val, "object");
  }
  Push(val);
}

void
JsonDeserializer::OnPairKeyBegin() {
  SetTag("key");
}

void
JsonDeserializer::OnPairValueBegin() {
  SetTag("value");
}

void
JsonDeserializer::OnPairEnd() {
  Pop();
}

//===------------------------------------------------------------------------===
// Primitive types
//===------------------------------------------------------------------------===

auto
JsonDeserializer::ReadBool() -> bool {
  auto *val = GetNextValue();
  if (!yyjson_is_bool(val)) {
    ThrowTypeError(val, "bool");
  }
  return yyjson_get_bool(val);
}

auto
JsonDeserializer::ReadSignedInt8() -> int8_t {
  auto *val = GetNextValue();
  if (!yyjson_is_int(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<int8_t>(yyjson_get_sint(val));
}

auto
JsonDeserializer::ReadUnsignedInt8() -> uint8_t {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<uint8_t>(yyjson_get_uint(val));
}

auto
JsonDeserializer::ReadSignedInt16() -> int16_t {
  auto *val = GetNextValue();
  if (!yyjson_is_int(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<int16_t>(yyjson_get_sint(val));
}

auto
JsonDeserializer::ReadUnsignedInt16() -> uint16_t {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<uint16_t>(yyjson_get_uint(val));
}

auto
JsonDeserializer::ReadSignedInt32() -> int32_t {
  auto *val = GetNextValue();
  if (!yyjson_is_int(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<int32_t>(yyjson_get_sint(val));
}

auto
JsonDeserializer::ReadUnsignedInt32() -> uint32_t {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<uint32_t>(yyjson_get_uint(val));
}

auto
JsonDeserializer::ReadSignedInt64() -> int64_t {
  auto *val = GetNextValue();
  if (!yyjson_is_int(val)) {
    ThrowTypeError(val, "int");
  }
  return static_cast<int64_t>(yyjson_get_sint(val));
}

auto
JsonDeserializer::ReadUnsignedInt64() -> uint64_t {
  auto *val = GetNextValue();
  if (!yyjson_is_uint(val)) {
    ThrowTypeError(val, "uint");
  }
  return static_cast<uint64_t>(yyjson_get_uint(val));
}

auto
JsonDeserializer::ReadFloat() -> float {
  auto *val = GetNextValue();
  if (!yyjson_is_real(val)) {
    ThrowTypeError(val, "float");
  }
  return static_cast<float>(yyjson_get_real(val));
}

auto
JsonDeserializer::ReadDouble() -> double {
  auto *val = GetNextValue();
  if (!yyjson_is_real(val)) {
    ThrowTypeError(val, "double");
  }
  return yyjson_get_real(val);
}

auto
JsonDeserializer::ReadString() -> string {
  auto *val = GetNextValue();
  if (!yyjson_is_str(val)) {
    ThrowTypeError(val, "string");
  }
  return {yyjson_get_str(val), yyjson_get_len(val)};
}

} // namespace estd
