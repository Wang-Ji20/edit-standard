//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/serializer_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "serializer/json_serializer.hh"
#include "string_utils.hh"

#include "gtest/gtest.h"

TEST(SerializerTest, SimpleJsonSerializerTest) {
  using namespace estd;
  std::unique_ptr<Serializer> serializer = std::make_unique<JsonSerializer>();
  uint8_t One = 1;

  serializer->OnObjectBegin();
  serializer->WriteProperty("test", One);
  serializer->OnObjectEnd();
  auto result = serializer->Serialize();
  serializer->Clear();

  ASSERT_TRUE(StringUtils::EqualIgnoreSpaceTabCRLF(
      std::string(result.begin(), result.end()), "[{\"test\":1}]"));
}

TEST(SerializerTest, DoubleJsonSerializeTest) {
  using namespace estd;
  std::unique_ptr<Serializer> serializer = std::make_unique<JsonSerializer>();
  uint8_t One = 1;
  serializer->OnObjectBegin();
  serializer->WriteProperty("test", One);
  serializer->OnObjectEnd();

  auto result = serializer->Serialize();
  serializer->Clear();

  ASSERT_TRUE(StringUtils::EqualIgnoreSpaceTabCRLF(
      std::string(result.begin(), result.end()), "[{\"test\":1}]"));

  uint8_t Two = 2;
  serializer->OnObjectBegin();
  serializer->WriteProperty("test", Two);
  serializer->OnObjectEnd();
  result = serializer->Serialize();
  serializer->Clear();

  ASSERT_TRUE(StringUtils::EqualIgnoreSpaceTabCRLF(
      std::string(result.begin(), result.end()), "[{\"test\":2}]"));
}