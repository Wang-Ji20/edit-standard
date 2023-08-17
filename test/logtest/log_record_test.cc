//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/log_record_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "logid.hh"
#include "log_record.hh"
#include "serializer/json_serializer.hh"

#include "gtest/gtest.h"

TEST(LogRecordSuite, LogRecordSerializeTest) {
  using namespace estd;
  LogRecord record;
  LogIDGenerator generator;
  record.id = generator.Generate();
  record.location.rawLocation = "$[trackName][][][].property";
  record.type = LogType::kCreate;
  record.data = {'a', 'b', 'c'};
  auto json = JsonSerializer::ToString(record);
  std::cerr << json << std::endl;
}
