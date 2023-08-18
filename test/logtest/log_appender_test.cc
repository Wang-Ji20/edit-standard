//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/log_appender_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "log_appender.hh"
#include "writer/ostream_writer.hh"
#include "serializer/json_serializer.hh"

#include "gtest/gtest.h"

estd::SerializerPrototype JSONSerializerFactory = []() {
  return std::make_unique<estd::JsonSerializer>();
};

TEST(LogAppenderSuite, AppendTest) {
  using namespace estd;
  using namespace std;
  OStreamWriter writer(cerr, JSONSerializerFactory);
  LogAppender appender(writer);

  vector<uint8_t> data{1, 2, 3, 4, 5};
  OtioLocation location{"$[root][][][].property"};
  appender.Append(LogType::kCreate, location, data);
  appender.Append(LogType::kModify, location, data);
}