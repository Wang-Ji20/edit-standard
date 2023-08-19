//===------------------------------------------===
// estd 2023
//
// Identification: include/log_appender.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_LOG_APPENDER_HH
#define ESTD_LOG_APPENDER_HH

#include "log_record.hh"
#include "writer/writer.hh"

#include <array>
#include <ctime>
#include <fstream>

namespace estd {

class LogAppender {
public:
  LogAppender(Writer &writer) : writer_(writer) {
    std::srand(std::time(nullptr));
    seed_ = std::rand();
  }

  LogAppender(uint64_t seed, Writer &writer) : seed_(seed), writer_(writer) {}

  void
  Append(LogType type,
         const OtioLocation &location,
         std::vector<uint8_t> &data) {
    LogID id(seed_, counter_++);
    LogRecord record{id, location, type, data};
    Append(std::move(record));
  }

  void
  Append(LogRecord &&record) {
    record.location.object.Accept(record);
    writer_.Write(record);
  }

private:
  uint64_t seed_;
  uint64_t counter_{};
  Writer &writer_;
};

} // namespace estd

#endif // ESTD_LOG_APPENDER_HH
