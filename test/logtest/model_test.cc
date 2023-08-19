//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/model_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "model/respository.hh"
#include "model/timeline.hh"
#include "serializer/json_serializer.hh"
#include "writer/ostream_writer.hh"

#include "writer_factory.hh"
#include "gtest/gtest.h"

TEST(ModelTest, RepoTest) {
  using namespace estd;
  using namespace std;
  auto [writer, fs] = WriterFactory::MakeFileWriter("112233.out");
  auto repo = make_unique<Respository>(*writer);
  repo->AddNewTimeline();
  writer->Write(*repo);
}

TEST(ModelTest, TimelineTest) {
  using namespace estd;
  using namespace std;
  namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;
  auto writer = WriterFactory::MakeConsoleWriter();
  auto repo = make_unique<Respository>(*writer);
  repo->AddNewTimeline();
  auto &timeline = repo->GetTimeline();
  timeline.SetGlobalStartTime(otio::RationalTime(1, 24));
  writer->Write(*repo);
}
