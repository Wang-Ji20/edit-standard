//===------------------------------------------===
// estd 2023
//
// Identification: include/serializer/deserializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------------------------------===
// The deserializer interface.
//
// This portion of code is (mostly) taken from DuckDB with some modification.
//===------------------------------------------------------------------------===

#pragma once

#include "serializer/serializer_type_traits.hh"
#include <cstdint>
#include <string>

namespace estd {
using std::string;
using std::make_unique;
using std::make_shared;

template <typename T, template <typename S = T> class Predicate = is_unique_ptr>
using ReturnType = typename std::enable_if<Predicate<T>::value, T>::type;

template <typename T, typename S>
using IsType = typename std::enable_if<std::is_same<T, S>::value, T>::type;

class Deserializer {
public:
  template <typename T> inline void ReadProperty(const char *tag, T &ret) {
    SetTag(tag);
    ret = Read<T>();
  }

  template <typename T> inline auto ReadProperty(const char *tag) -> T {
    SetTag(tag);
    return Read<T>();
  }

  //===------------------------------------------------------------------------===
  // class interface, override by concrete deserializer
  //===------------------------------------------------------------------------===
  virtual void SetTag(const char *tag) = 0;
  virtual auto OnVectorBegin() -> size_t = 0;
  virtual void OnVectorEnd() {}
  virtual auto OnMapBegin() -> size_t = 0;
  virtual void OnMapEnd() {}
  virtual void OnMapEntryBegin() {}
  virtual void OnMapEntryEnd() {}
  virtual void OnMapKeyBegin() {}
  virtual void OnMapKeyEnd() {}
  virtual void OnMapValueBegin() {}
  virtual void OnMapValueEnd() {}
  virtual void OnObjectBegin() {}
  virtual void OnObjectEnd() {}
  virtual void OnPairBegin() {}
  virtual void OnPairKeyBegin() {}
  virtual void OnPairKeyEnd() {}
  virtual void OnPairValueBegin() {}
  virtual void OnPairValueEnd() {}
  virtual void OnPairEnd() {}

protected:
  virtual auto ReadBool() -> bool = 0;
  virtual auto ReadSignedInt8() -> int8_t = 0;
  virtual auto ReadUnsignedInt8() -> uint8_t = 0;
  virtual auto ReadSignedInt16() -> int16_t = 0;
  virtual auto ReadUnsignedInt16() -> uint16_t = 0;
  virtual auto ReadSignedInt32() -> int32_t = 0;
  virtual auto ReadUnsignedInt32() -> uint32_t = 0;
  virtual auto ReadSignedInt64() -> int64_t = 0;
  virtual auto ReadUnsignedInt64() -> uint64_t = 0;
  virtual auto ReadFloat() -> float = 0;
  virtual auto ReadDouble() -> double = 0;
  virtual auto ReadString() -> string = 0;

  //===------------------------------------------------------------------------===
  // Internal templates
  //===------------------------------------------------------------------------===
public:
  void SetTag(const string &tag) { SetTag(tag.c_str()); };

  /// read a generic value
  template <typename T> auto Read() -> ReturnType<T, is_deserializable> {
    // NOTE: This thing must be a static function!
    // static
    return T::estdReadValue(*this);
  }

  template <typename T> auto Read() -> ReturnType<T, is_unique_ptr> {
    using ELEMENT_TYPE = typename is_unique_ptr<T>::ELEMENT_TYPE;
    return make_unique<ELEMENT_TYPE>(Read<ELEMENT_TYPE>());
  }

  template <typename T> auto Read() -> ReturnType<T, is_shared_ptr> {
    using ELEMENT_TYPE = typename is_shared_ptr<T>::ELEMENT_TYPE;
    return make_shared<ELEMENT_TYPE>(Read<ELEMENT_TYPE>());
  }

  template <typename T> auto Read() -> ReturnType<T, is_vector> {
    using ELEMENT_TYPE = typename is_vector<T>::ELEMENT_TYPE;
    T ret;
    auto size = OnVectorBegin();
    for (auto i = 0ULL; i < size; ++i) {
      ret.push_back(Read<ELEMENT_TYPE>());
    }
    OnVectorEnd();
    return ret;
  }

  template <typename T> auto Read() -> ReturnType<T, is_unordered_map> {
    using KEY_TYPE = typename is_unordered_map<T>::KEY_TYPE;
    using VALUE_TYPE = typename is_unordered_map<T>::VALUE_TYPE;
    T ret;
    auto size = OnMapBegin();
    for (auto i = 0ULL; i < size; ++i) {
      OnMapEntryBegin();
      OnMapKeyBegin();
      auto key = Read<KEY_TYPE>();
      OnMapKeyEnd();
      OnMapValueBegin();
      auto value = Read<VALUE_TYPE>();
      OnMapValueEnd();
      OnMapEntryEnd();
      ret[std::move(key)] = std::move(value);
    }
    OnMapEnd();
    return ret;
  }

  template <typename T> auto Read() -> ReturnType<T, is_unordered_set> {
    using ELEMENT_TYPE = typename is_unordered_set<T>::ELEMENT_TYPE;
    T ret;
    auto size = OnVectorBegin();
    for (auto i = 0ULL; i < size; ++i) {
      ret.insert(Read<ELEMENT_TYPE>());
    }
    OnVectorEnd();
    return ret;
  }

  template <typename T> auto Read() -> ReturnType<T, is_pair> {
    using FIRST_TYPE = typename is_pair<T>::FIRST_TYPE;
    using SECOND_TYPE = typename is_pair<T>::SECOND_TYPE;
    OnPairBegin();
    OnPairKeyBegin();
    auto first = Read<FIRST_TYPE>();
    OnPairKeyEnd();
    OnPairValueBegin();
    auto second = Read<SECOND_TYPE>();
    OnPairValueEnd();
    OnPairEnd();
    return std::make_pair(std::move(first), std::move(second));
  }

  //===------------------------------------------------------------------------===
  // primitive types
  //===------------------------------------------------------------------------===

  template <typename T> auto Read() -> IsType<T, bool> { return ReadBool(); }

  template <typename T> auto Read() -> IsType<T, int8_t> {
    return ReadSignedInt8();
  }

  template <typename T> auto Read() -> IsType<T, uint8_t> {
    return ReadUnsignedInt8();
  }

  template <typename T> auto Read() -> IsType<T, int16_t> {
    return ReadSignedInt16();
  }

  template <typename T> auto Read() -> IsType<T, uint16_t> {
    return ReadUnsignedInt16();
  }

  template <typename T> auto Read() -> IsType<T, int32_t> {
    return ReadSignedInt32();
  }

  template <typename T> auto Read() -> IsType<T, uint32_t> {
    return ReadUnsignedInt32();
  }

  template <typename T> auto Read() -> IsType<T, int64_t> {
    return ReadSignedInt64();
  }

  template <typename T> auto Read() -> IsType<T, uint64_t> {
    return ReadUnsignedInt64();
  }

  template <typename T> auto Read() -> IsType<T, float> { return ReadFloat(); }

  template <typename T> auto Read() -> IsType<T, double> {
    return ReadDouble();
  }

  template <typename T> auto Read() -> IsType<T, string> {
    return ReadString();
  }

  template <typename T> auto Read() -> ReturnType<T, std::is_enum> {
    using UNDERLYING_TYPE = typename std::underlying_type<T>::type;
    return static_cast<T>(Read<UNDERLYING_TYPE>());
  }
};

class ParseException : public std::runtime_error {
public:
  static constexpr const char *DEFAULT_MESSAGE = "[Parse exception]";
  explicit ParseException(const string &message)
      : std::runtime_error(DEFAULT_MESSAGE + message) {}
};

} // namespace estd
