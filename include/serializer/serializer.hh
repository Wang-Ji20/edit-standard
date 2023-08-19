//===------------------------------------------===
// estd 2023
//
// Identification: include/serializer/serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// The serializer interface is only used to show the
// layout of data. It is not responsible for the
// specific formatting of the data.
//
// Such thing is left to the concrete serializer, and
// caller should aware what type of serializer is used in
// storage module.
//
// Responsibility:
//  - serialize data into a byte stream
//
// Serializer state machine:
//  1 empty
//  2 serialized something
//
//  1 -> 2 call Write related func
//  2 -> 2 call Write related func
//  2 -> 1 call Clear()
//
//===------------------------------------------------===

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <set>

namespace estd {
using std::string;
using std::unique_ptr;

class Serializer {
  friend class Writer;

public:
  virtual auto
  Serialize() -> std::vector<uint8_t> = 0;
  virtual void
  Clear() = 0;

  virtual void
  SetTag(const char *tag) = 0;
  virtual ~Serializer() = default;
  /// write a generic value
  // Friend Template Argument-Dependent Lookup Extension
  //
  // Usage:
  // --------------------
  //
  // in a user-defined type:
  //
  // class xxx {
  //
  // template <typename Ser>
  // friend void estdWriteValue(Ser& serializer, const xxx& value) {
  //    put the serialization result into serializer
  // }
  //
  // };
  //
  // Note:
  // --------------------
  // 1. the user-defined type can be a scalar or a complex type, so we left
  //    OnObjectBegin() and OnObjectEnd() to the user.
  //
  template <typename T>
  void
  WriteValue(const T &value) {
    estdWriteValue(*this, value);
  }

  /// write a unique_ptr
  template <typename T>
  void
  WriteValue(const unique_ptr<T> &ptr) {
    WriteValue(ptr.get());
  }

  /// write a pointer
  template <typename T>
  auto
  WriteValue(const T ptr) ->
      typename std::enable_if<std::is_pointer<T>::value, void>::type {
    if (ptr == nullptr) {
      WriteNull();
    } else {
      WriteValue(*ptr);
    }
  }

  /// write properties
  /// syntax sugar for "key" : "value"
  template <typename T>
  void
  WriteProperty(const char *key, const T &value) {
    SetTag(key);
    if constexpr (!std::is_enum_v<T>) {
      WriteValue(value);
      return;
    } else {
      WriteValue(static_cast<typename std::underlying_type<T>::type>(value));
    }
  }

  //===------------------------------------------------------------------------===
  // These part is used by Data records types mainly.
  //===------------------------------------------------------------------------===

  virtual void
  OnObjectBegin() {}
  virtual void
  OnObjectEnd() {}

  //===------------------------------------------------===
  // write a pair
  //===------------------------------------------------===
  virtual void
  OnPairBegin() {}
  virtual void
  OnPairKeyBegin() {}
  virtual void
  OnPairKeyEnd() {}
  virtual void
  OnPairValueBegin() {}
  virtual void
  OnPairValueEnd() {}
  virtual void
  OnPairEnd() {}

  template <typename K, typename V>
  void
  WriteValue(const std::pair<K, V> &pair) {
    OnPairBegin();
    OnPairKeyBegin();
    WriteValue(pair.first);
    OnPairKeyEnd();
    OnPairValueBegin();
    WriteValue(pair.second);
    OnPairValueEnd();
    OnPairEnd();
  }

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  virtual void
  OnVectorBegin(size_t size) {}
  virtual void
  OnVectorEnd(size_t size) {}
  virtual void
  OnVectorItemBegin(size_t maxSize, size_t curSize) {}
  virtual void
  OnVectorItemEnd(size_t maxSize, size_t curSize) {}

  template <typename T>
  void
  WriteValue(const std::vector<T> &vec) {
    const size_t vecSize = vec.size();
    OnVectorBegin(vecSize);
    for (auto i = 0ULL; i < vecSize; ++i) {
      OnVectorItemBegin(vecSize, i);
      WriteValue(vec[i]);
      OnVectorItemEnd(vecSize, i);
    }
    OnVectorEnd(vecSize);
  }

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  virtual void
  OnUnorderedMapBegin(size_t size) {}
  virtual void
  OnUnorderedMapEnd(size_t size) {}
  virtual void
  OnUnorderedMapItemBegin() {}
  virtual void
  OnUnorderedMapItemEnd() {}
  virtual void
  OnUnorderedMapKeyBegin() {}
  virtual void
  OnUnorderedMapKeyEnd() {}
  virtual void
  OnUnorderedMapValueBegin() {}
  virtual void
  OnUnorderedMapValueEnd() {}

  template <typename K, typename V>
  void
  WriteValue(const std::unordered_map<K, V> &map) {
    const size_t mapSize = map.size();
    OnUnorderedMapBegin(mapSize);
    for (auto &item : map) {
      OnUnorderedMapItemBegin();
      OnUnorderedMapKeyBegin();
      WriteValue(item.first);
      OnUnorderedMapKeyEnd();
      OnUnorderedMapValueBegin();
      WriteValue(item.second);
      OnUnorderedMapValueEnd();
      OnUnorderedMapItemEnd();
    }
    OnUnorderedMapEnd(mapSize);
  }

  //===------------------------------------------------===
  // write a set
  //===------------------------------------------------===
  virtual void
  OnSetBegin(size_t size) {}
  virtual void
  OnSetEnd(size_t size) {}
  virtual void
  OnSetItemBegin() {}
  virtual void
  OnSetItemEnd() {}

  template <typename T>
  void
  WriteValue(const std::set<T> &set) {
    const size_t setSize = set.size();
    OnSetBegin(setSize);
    for (auto &item : set) {
      OnSetItemBegin();
      WriteValue(item);
      OnSetItemEnd();
    }
    OnSetEnd(setSize);
  }

  //===------------------------------------------------===
  // primitive types
  //===------------------------------------------------===

  virtual void
  WriteValue(const string &value);
  virtual void
  WriteValue(const char *value) = 0;
  virtual void
  WriteNull() = 0;
  virtual void
  WriteValue(bool value) = 0;
  virtual void
  WriteValue(uint8_t value) = 0;
  virtual void
  WriteValue(int8_t value) = 0;
  virtual void
  WriteValue(uint16_t value) = 0;
  virtual void
  WriteValue(int16_t value) = 0;
  virtual void
  WriteValue(uint32_t value) = 0;
  virtual void
  WriteValue(int32_t value) = 0;
  virtual void
  WriteValue(uint64_t value) = 0;
  virtual void
  WriteValue(int64_t value) = 0;
  virtual void
  WriteValue(float value) = 0;
  virtual void
  WriteValue(double value) = 0;
};

} // namespace estd
