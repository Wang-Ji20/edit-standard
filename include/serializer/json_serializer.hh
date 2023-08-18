//===------------------------------------------===
// estd 2023
//
// Identification: include/serializer/json_serializer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "serializer/serializer.hh"

#include "yyjson.h"

#include <vector>

namespace estd {

class JsonSerializer : public Serializer {
  //===------------------------------------------------===
  // public interface
  //===------------------------------------------------===
public:
  explicit JsonSerializer(yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr),
                          bool pretty = true, bool skipNull = false,
                          bool skipEmpty = false)
      : doc(doc), stack{yyjson_mut_arr(doc)}, skipEmpty_(skipEmpty),
        pretty_(pretty) {
    yyjson_mut_doc_set_root(doc, GetCurrent());
  }

  auto Serialize() -> std::vector<uint8_t> override {
    auto *str =
        yyjson_mut_write(doc, pretty_ ? YYJSON_WRITE_PRETTY : 0, nullptr);
    auto result = std::vector<uint8_t>(str, str + strlen(str));
    free(str);
    return result;
  }

  auto Clear() -> void override {
    yyjson_mut_doc_free(doc);
    buffer_.clear();
    stack.clear();
    doc = yyjson_mut_doc_new(nullptr);
    stack.push_back(yyjson_mut_arr(doc));
    yyjson_mut_doc_set_root(doc, GetCurrent());
  }

  ~JsonSerializer() override { yyjson_mut_doc_free(doc); }

  //===------------------------------------------------===
  // Internals
  //===------------------------------------------------===

private:
  yyjson_mut_doc *doc;
  yyjson_mut_val *current_tag;

  std::vector<yyjson_mut_val *> stack;
  auto GetRoot() -> yyjson_mut_val * { return stack.front(); }
  inline auto GetCurrent() -> yyjson_mut_val * { return stack.back(); }

  bool skipEmpty_ = false;
  bool pretty_ = true;

  void PushValue(yyjson_mut_val *val);
  void PruneEmptyObject(yyjson_mut_val *obj);

  //===------------------------------------------------===
  // Serializer interface
  //===------------------------------------------------===

public:
  void SetTag(const char *tag) override;

  void OnObjectBegin() final;
  void OnObjectEnd() final;

  //===------------------------------------------------===
  // write a pair
  //===------------------------------------------------===
  void OnPairBegin() final;
  void OnPairKeyBegin() final;
  void OnPairValueBegin() final;
  void OnPairEnd() final;

  //===------------------------------------------------===
  // write a vector
  //===------------------------------------------------===
  void OnVectorBegin(size_t size) final;
  void OnVectorEnd(size_t size) final;

  //===------------------------------------------------===
  // write an unordered map
  //===------------------------------------------------===
  void OnUnorderedMapBegin(size_t size) final;
  void OnUnorderedMapEnd(size_t size) final;
  void OnUnorderedMapItemBegin() final;
  void OnUnorderedMapItemEnd() final;
  void OnUnorderedMapKeyBegin() final;
  void OnUnorderedMapValueBegin() final;

  // https://stackoverflow.com/questions/3678197/virtual-function-implemented-in-base-class-not-being-found-by-compiler
  using Serializer::WriteValue;
  void WriteValue(const char *value) final;
  void WriteNull() final;
  void WriteValue(bool value) final;
  void WriteValue(uint8_t value) final;
  void WriteValue(int8_t value) final;
  void WriteValue(uint16_t value) final;
  void WriteValue(int16_t value) final;
  void WriteValue(uint32_t value) final;
  void WriteValue(int32_t value) final;
  void WriteValue(uint64_t value) final;
  void WriteValue(int64_t value) final;
  void WriteValue(float value) final;
  void WriteValue(double value) final;

private:
  std::vector<uint8_t> buffer_;
};

} // namespace estd
