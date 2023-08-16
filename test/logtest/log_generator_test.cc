//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/log_generator_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "logid.hh"

#include "gtest/gtest.h"

TEST(LogIDGeneratorSuite, GenerateTest) {
  estd::LogIDGenerator generator;
  auto id = generator.Generate();
  EXPECT_EQ(id.size(), 16);
  EXPECT_EQ(id[15], 0);
  std::cerr << "LogID: ";
  for (auto c : id) {
    std::cerr << std::hex << (int)c;
  }
  std::cerr << std::endl;
}