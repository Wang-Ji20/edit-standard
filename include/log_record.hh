//===------------------------------------------===
// estd 2023
//
// Identification: include/log_record.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------------------------------===
// Record types for Logs. They hold the responsibility to maintain some
// contracts, properties and describe themselves to the formatting
// infrastructure.
//===------------------------------------------------------------------------===

#ifndef ESTD_LOG_RECORD_HH
#define ESTD_LOG_RECORD_HH

#include "memory.hh"
#include "model/abstract_model.hh"

#include "opentimelineio/serializableObject.h"
#include <string>
#include <utility>

namespace estd {

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

enum class OTIOType : uint8_t {
  kInvalid = 0,
  kDummy,
  kTimeline,
  kStack,
  kTrack,
  kClip,
  kGap,
  kTransition,
  kMarker,
  kEffect,
};

struct OtioLocation {
  OtioLocation(std::string rawLocation,
               OTIOType objectTy,
               AbstractModel &aobject)
      : objectTy(objectTy), object(aobject) {
    Validate(rawLocation);
    this->rawLocation = std::move(rawLocation);
  }

  auto
  operator==(const OtioLocation &other) const -> bool {
    return rawLocation == other.rawLocation;
  }

  static void
  Validate(const std::string &rawLocation) {
    if (rawLocation.empty()) {
      throw std::invalid_argument("empty location");
    }
    if (rawLocation[0] != '$') {
      throw std::invalid_argument("must start with $");
    }
    // TODO: more sanity checks.
  }

  template <typename Ser>
  friend void
  estdWriteValue(Ser &serializer, const OtioLocation &value) {
    serializer.WriteValue(value.rawLocation);
  }

  // use $[trackName][][][].property to refer an OTIO object
  std::string rawLocation;
  OTIOType objectTy;
  AbstractModel &object;
};

enum class LogType : uint8_t {
  kInvalid = 0,
  kDummy,
  kCreate, // object type + object serialized data
  kModify, // field name + field value
  kDelete, // object type
  kAppend, // sorce object + object type + object serialized data
  kErase,  // source object + object type + index
};

struct LogID {
  LogID(uint64_t seed, uint64_t order) {
    std::memcpy(id_.data(), &seed, sizeof(uint64_t));
    std::memcpy(id_.data() + sizeof(uint64_t), &order, sizeof(uint64_t));
  }

  auto
  Data() -> uint8_t * {
    return id_.data();
  }
  auto
  operator==(const LogID &other) const -> bool {
    return id_ == other.id_;
  }

  auto
  operator>(const LogID &other) const -> bool {
    return id_ > other.id_;
  }

  [[nodiscard]] auto
  Order() const -> uint64_t {
    auto order = MemoryUtils::Load<uint64_t>(id_.data(), sizeof(uint64_t));
    return order;
  }

  template <typename Ser>
  friend void
  estdWriteValue(Ser &serializer, const LogID &value) {
    serializer.OnVectorBegin(16);
    for (auto c : value.id_) {
      serializer.WriteValue(c);
    }
    serializer.OnVectorEnd(16);
  }

  std::array<uint8_t, 16> id_;
};

struct LogRange {
  LogID begin;
  LogID end;

  template <typename Ser>
  friend void
  estdWriteValue(Ser &serializer, const LogRange &value) {
    serializer.OnObjectBegin();
    serializer.WriteProperty("begin", value.begin);
    serializer.WriteProperty("end", value.end);
    serializer.OnObjectEnd();
  }
};

struct LogRecord {
  LogID id;
  OtioLocation location;
  LogType type;
  std::vector<uint8_t> data;

  template <typename Ser>
  friend void
  estdWriteValue(Ser &serializer, const LogRecord &value) {
    serializer.OnObjectBegin();
    serializer.WriteProperty("id", value.id);
    serializer.WriteProperty("location", value.location);
    serializer.WriteProperty("type", value.type);
    serializer.WriteProperty("data", value.data);
    serializer.OnObjectEnd();
  }
};

} // namespace estd

#endif // ESTD_LOG_RECORD_HH
