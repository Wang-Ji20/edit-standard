//===------------------------------------------===
// estd 2023
//
// Identification: include/model/timeline.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_MODEL_TIMELINE_HH
#define ESTD_MODEL_TIMELINE_HH

#include "log_appender.hh"
#include "opentime/rationalTime.h"
#include "opentimelineio/timeline.h"
#include "otio_tracker.hh"

namespace estd {

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

class Timeline : public AbstractModel {
  friend struct TimelineModifyExecutor;

public:
  Timeline(LogAppender &appender)
      : timeline_(new otio::Timeline()), appender_(appender) {
    timeline_->set_global_start_time(otio::RationalTime(0, 0));
  }
  ~Timeline() override = default;

  template <typename Ser>
  friend void
  estdWriteValue(Ser &serializer, const Timeline &value) {
    serializer.WriteValue(value.timeline_->to_json_string());
  }

  void
  Accept(LogRecord &action) override;

  void
  SetGlobalStartTime(const opentime::RationalTime &time);

private:
  otio::SerializableObject::Retainer<otio::Timeline> timeline_;
  OTIOTracker tracker_ = OTIOTracker::NewTrackerFromRoot(*this);
  LogAppender &appender_;
};

} // namespace estd

#endif // ESTD_MODEL_TIMELINE_HH
