//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/log_appender_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "log_appender.hh"
#include "model/respository.hh"
#include "serializer/json_serializer.hh"
#include "writer/ostream_writer.hh"

#include "gtest/gtest.h"

estd::SerializerPrototype JSONSerializerFactory = []() {
  return std::make_unique<estd::JsonSerializer>();
};

TEST(LogAppenderSuite, AppendTest) {
  using namespace estd;
  using namespace std;
  OStreamWriter writer(cerr, JSONSerializerFactory);
  LogAppender appender(writer);

  Respository repo(writer);
  OtioLocation mockdummyLoc{"$", OTIOType::kDummy, repo};
  vector<uint8_t> mockdummydata;

  appender.Append(LogType::kDummy, mockdummyLoc, mockdummydata);
  EXPECT_ANY_THROW(
      appender.Append(LogType::kModify, mockdummyLoc, mockdummydata));
}
