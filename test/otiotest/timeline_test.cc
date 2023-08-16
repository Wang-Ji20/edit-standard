//===------------------------------------------===
// estd 2023
//
// Identification: test/otiotest/timeline_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "opentimelineio/timeline.h"
#include "gtest/gtest.h"

#include "timeline_factory.hh"

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

TEST(TimelineManipulateSuite, NewTimelineTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.ValidateTimeline());
}

TEST(TimelineManipulateSuite, AddTrackTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.CreateTrack());
  builder.CompositeTrack();
  EXPECT_NO_THROW(builder.ValidateTrack());
}

TEST(TimelineManipulateSuite, AddStackTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.CreateTrack());
  EXPECT_NO_THROW(builder.CreateStack());
  builder.CompositeTrack();
  builder.CompositeStack();
  EXPECT_NO_THROW(builder.ValidateStack());
}

TEST(TimelineManipulateSuite, AddGapTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.CreateTrack());
  EXPECT_NO_THROW(builder.CreateStack());
  EXPECT_NO_THROW(builder.CreateGap());
  builder.CompositeTrack();
  builder.CompositeStack();
  builder.CompositeGap();
  EXPECT_NO_THROW(builder.ValidateGap());
}

TEST(TimelineManipulateSuite, AddTransitionTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.CreateTrack());
  EXPECT_NO_THROW(builder.CreateStack());
  EXPECT_NO_THROW(builder.CreateTransition());
  builder.CompositeTrack();
  builder.CompositeStack();
  builder.CompositeTransition();
  EXPECT_NO_THROW(builder.ValidateTransition());
}

TEST(TimelineManipulateSuite, AddClipTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.CreateTrack());
  EXPECT_NO_THROW(builder.CreateStack());
  EXPECT_NO_THROW(builder.CreateClip());
  builder.CompositeTrack();
  builder.CompositeStack();
  builder.CompositeClip();
  EXPECT_NO_THROW(builder.ValidateClip());
}

TEST(TimelineManipulateSuite, AddMarkerTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.CreateTrack());
  EXPECT_NO_THROW(builder.CreateStack());
  EXPECT_NO_THROW(builder.CreateMarker());
  builder.CompositeTrack();
  builder.CompositeStack();
  builder.CompositeMarker();
  EXPECT_NO_THROW(builder.ValidateMarker());
}

TEST(TimelineManipulateSuite, AddEffectTest) {
  auto builder = TestSimpleTimelineBuilder();
  auto op = builder.CreateTimeline();
  EXPECT_NO_THROW(builder.CreateTrack());
  EXPECT_NO_THROW(builder.CreateStack());
  EXPECT_NO_THROW(builder.CreateEffect());
  builder.CompositeTrack();
  builder.CompositeStack();
  builder.CompositeEffect();
  EXPECT_NO_THROW(builder.ValidateEffect());
}

TEST(TimelineManipulateSuite, BuildReadyTest) {
  EXPECT_NO_THROW(auto retainer =
                      TestSimpleTimelineBuilder::BuildAReadyTimeline());
}

TEST(TimelineManipulateSuite, DISABLED_PrintDummy) {
  auto retainer = TestSimpleTimelineBuilder::BuildAReadyTimeline();
  auto tl = dynamic_cast<otio::Timeline*>(retainer.value);
  std::cerr << tl->to_json_string();
}
