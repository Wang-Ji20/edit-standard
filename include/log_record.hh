//===------------------------------------------===
// estd 2023
//
// Identification: include/log_record.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_LOG_RECORD_HH
#define ESTD_LOG_RECORD_HH

#include <string>
#include "opentimelineio/serializableObject.h"
#include "logid.hh"

namespace estd {

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

struct OtioLocation {
  // use $[trackName][][][].property to refer an OTIO object
  std::string rawLocation;

  template <typename Ser>
  friend void estdWriteValue(Ser &serializer, const OtioLocation &value) {
    serializer.WriteValue(value.rawLocation);
  }
};

enum class LogType: uint8_t {
  kCreate, // object type + object serialized data
  kModify, // field name + field value
  kDelete, // object type
  kAppend, // sorce object + object type + object serialized data
  kErase,  // source object + object type + index
};

struct LogRecord {
  LogID id;
  OtioLocation location;
  LogType type;
  std::vector<uint8_t> data;

  template <typename Ser>
  friend void estdWriteValue(Ser &serializer, const LogRecord &value) {
    serializer.OnObjectBegin();
    serializer.WriteProperty("id", value.id);
    serializer.WriteProperty("location", value.location);
    serializer.WriteProperty("type", value.type);
    serializer.WriteProperty("data", value.data);
    serializer.OnObjectEnd();
  }
};

} // namespace estd

#endif
