//===------------------------------------------===
// estd 2023
//
// Identification: test/otiotest/timeline_factory.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "opentimelineio/clip.h"
#include "opentimelineio/effect.h"
#include "opentimelineio/gap.h"
#include "opentimelineio/marker.h"
#include "opentimelineio/timeline.h"
#include "opentimelineio/transition.h"
#include "gtest/gtest.h"

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

//===------------------------------------------------------------------------===
// Create a sample timeline as below structure:
//
//  Timeline:
//    test_track:
//        test_stack:
//          test_gap:
//          test_transition:
//          test_clip:
//          test_marker:
//          test_effect:
//===------------------------------------------------------------------------===

class TestSimpleTimelineBuilder {
public:
  TestSimpleTimelineBuilder() = default;
  ~TestSimpleTimelineBuilder() = default;

  static auto BuildAReadyTimeline()
      -> otio::SerializableObject::Retainer<otio::Timeline> {
    auto builder = TestSimpleTimelineBuilder();
    builder.CreateTimeline();
    builder.CreateTrack();
    builder.CreateStack();
    builder.CreateGap();
    builder.CreateTransition();
    builder.CreateClip();
    builder.CreateMarker();
    builder.CreateEffect();
    builder.DummyComposite();
    builder.ValidateTimeline();
    return builder.tl;
  }

  auto CreateTimeline() -> otio::SerializableObject::Retainer<otio::Timeline> {
    tl = new otio::Timeline();
    tl->set_name("test_timeline");
    return tl;
  }

  auto CreateTrack() -> otio::SerializableObject::Retainer<otio::Track> {
    track = new otio::Track();
    track->set_name("test_track");
    return track;
  }

  auto CreateClip() -> otio::SerializableObject::Retainer<otio::Clip> {
    clip = new otio::Clip();
    clip->set_name("test_clip");
    return clip;
  }

  auto CreateStack() -> otio::SerializableObject::Retainer<otio::Stack> {
    stack = new otio::Stack();
    stack->set_name("test_stack");
    return stack;
  }

  auto CreateGap() -> otio::SerializableObject::Retainer<otio::Gap> {
    gap = new otio::Gap();
    gap->set_name("test_gap");
    return gap;
  }

  auto CreateTransition()
      -> otio::SerializableObject::Retainer<otio::Transition> {
    transition = new otio::Transition();
    transition->set_name("test_transition");
    return transition;
  }

  auto CreateMarker() -> otio::SerializableObject::Retainer<otio::Marker> {
    marker = new otio::Marker();
    marker->set_name("test_marker");
    return marker;
  }

  auto CreateEffect() -> otio::SerializableObject::Retainer<otio::Effect> {
    effect = new otio::Effect();
    effect->set_name("test_effect");
    return effect;
  }

  void DummyComposite() {
    CompositeTimeline();
    CompositeTrack();
    CompositeStack();
    CompositeGap();
    CompositeTransition();
    CompositeMarker();
    CompositeEffect();
    CompositeClip();
  }

  void CompositeTimeline() { ValidateTimeline(); }

  void CompositeTrack() {
    ValidateTimeline();
    tl->tracks()->append_child(track);
  }

  void CompositeStack() {
    ValidateTimeline();
    ValidateTrack();
    track->append_child(stack);
  }

  void CompositeGap() {
    ValidateTimeline();
    ValidateTrack();
    ValidateStack();
    stack->append_child(gap);
  }

  void CompositeTransition() {
    ValidateTimeline();
    ValidateTrack();
    ValidateStack();
    stack->append_child(transition);
  }

  void CompositeMarker() {
    ValidateTimeline();
    ValidateTrack();
    ValidateStack();
    stack->markers().push_back(marker);
  }

  void CompositeEffect() {
    ValidateTimeline();
    ValidateTrack();
    ValidateStack();
    stack->effects().push_back(effect);
  }

  void CompositeClip() {
    ValidateTimeline();
    ValidateTrack();
    ValidateStack();
    stack->append_child(clip);
  }

  void ValidateComposite() {
    ValidateTimeline();
    ValidateTrack();
    ValidateStack();
    ValidateGap();
    ValidateTransition();
    ValidateMarker();
    ValidateEffect();
    ValidateClip();
  }

  void ValidateTimeline() {
    if (!tl) {
      throw std::runtime_error("Timeline is not created yet.");
    }
    if (!tl->tracks()) {
      throw std::runtime_error("Timeline has no stack. inconsistent state.");
    }
    if (tl->tracks()->name() != "tracks") {
      throw std::runtime_error(
          "Timeline stack has problem. inconsistent state.");
    }
  }

  void ValidateTrack() {
    if (!track) {
      throw std::runtime_error("Track is not created yet.");
    }
    if (!track->parent()) {
      throw std::runtime_error("Track has no parent. inconsistent state.");
    }
    if (track->parent()->name() != "tracks") {
      throw std::runtime_error("Track parent has problem. inconsistent state.");
    }
    if (track->name() != "test_track") {
      throw std::runtime_error(
          "Track name is not correct. inconsistent state.");
    }
  }

  void ValidateStack() {
    if (!stack) {
      throw std::runtime_error("Stack is not created yet.");
    }
    if (!stack->parent()) {
      throw std::runtime_error("Stack has no parent. inconsistent state.");
    }
    if (stack->parent()->name() != "test_track") {
      throw std::runtime_error("Stack parent has problem. inconsistent state.");
    }
    if (stack->name() != "test_stack") {
      throw std::runtime_error(
          "Stack name is not correct. inconsistent state.");
    }
  }

  void ValidateGap() {
    if (!gap) {
      throw std::runtime_error("Gap is not created yet.");
    }
    if (!gap->parent()) {
      throw std::runtime_error("Gap has no parent. inconsistent state.");
    }
    if (gap->parent()->name() != "test_stack") {
      throw std::runtime_error("Gap parent has problem. inconsistent state.");
    }
    if (gap->name() != "test_gap") {
      throw std::runtime_error("Gap name is not correct. inconsistent state.");
    }
  }

  void ValidateTransition() {
    if (!transition) {
      throw std::runtime_error("Transition is not created yet.");
    }
    if (!transition->parent()) {
      throw std::runtime_error("Transition has no parent. inconsistent state.");
    }
    if (transition->parent()->name() != "test_stack") {
      throw std::runtime_error(
          "Transition parent has problem. inconsistent state.");
    }
    if (transition->name() != "test_transition") {
      throw std::runtime_error(
          "Transition name is not correct. inconsistent state.");
    }
  }

  void ValidateMarker() {
    if (!marker) {
      throw std::runtime_error("Marker is not created yet.");
    }
    if (marker->name() != "test_marker") {
      throw std::runtime_error(
          "Marker name is not correct. inconsistent state.");
    }
  }

  void ValidateEffect() {
    if (!effect) {
      throw std::runtime_error("Effect is not created yet.");
    }
    if (effect->name() != "test_effect") {
      throw std::runtime_error(
          "Effect name is not correct. inconsistent state.");
    }
  }

  void ValidateClip() {
    if (!clip) {
      throw std::runtime_error("Clip is not created yet.");
    }
    if (!clip->parent()) {
      throw std::runtime_error("Clip has no parent. inconsistent state.");
    }
    if (clip->parent()->name() != "test_stack") {
      throw std::runtime_error("Clip parent has problem. inconsistent state.");
    }
    if (clip->name() != "test_clip") {
      throw std::runtime_error("Clip name is not correct. inconsistent state.");
    }
  }

private:
  otio::SerializableObject::Retainer<otio::Timeline> tl;
  otio::SerializableObject::Retainer<otio::Track> track;
  otio::SerializableObject::Retainer<otio::Clip> clip;
  otio::SerializableObject::Retainer<otio::Stack> stack;
  otio::SerializableObject::Retainer<otio::Gap> gap;
  otio::SerializableObject::Retainer<otio::Transition> transition;
  otio::SerializableObject::Retainer<otio::Marker> marker;
  otio::SerializableObject::Retainer<otio::Effect> effect;
};
