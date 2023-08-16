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

struct ObjLocation {
  // use $[trackName][][][].property to refer an OTIO object
  std::string rawLocation;
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
  ObjLocation location;
  LogType type;
  std::vector<char> data;
};

} // namespace estd

#endif
