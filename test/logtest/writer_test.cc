//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/writer_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "serializer/json_serializer.hh"
#include "string_utils.hh"
#include "writer/ostream_writer.hh"

#include "gtest/gtest.h"

struct SomeRecord {
  int x = 1;
  int y = 2;

  template <typename Ser>
  friend void estdWriteValue(Ser &ser, const SomeRecord &record) {
    ser.OnObjectBegin();
    ser.WriteProperty("x", record.x);
    ser.WriteProperty("y", record.y);
    ser.OnObjectEnd();
  }
};

TEST(WriterTest, SimpleTest) {
  using namespace estd;
  std::ostringstream oss;
  std::unique_ptr<Writer> writer = std::make_unique<OStreamWriter>(
      oss, []() { return std::make_unique<JsonSerializer>(); });
  SomeRecord record;
  writer->Write(record);
  EXPECT_TRUE(StringUtils::EqualIgnoreSpaceTabCRLF(
      oss.str(), "\x15|J\x7F\xE3q\xCC\x9C [{\"x\":1,\"y\":2}]"));
}