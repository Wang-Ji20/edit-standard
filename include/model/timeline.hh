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
#include "opentimelineio/timeline.h"
#include "otio_tracker.hh"

namespace estd {

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

class Timeline : public AbstractModel {
public:
  Timeline(LogAppender &appender) : appender_(appender) {}

  template <typename Ser>
  friend void estdWriteValue(Ser &serializer, const Timeline &value) {
    serializer.WriteValue(value.timeline_->to_json_string());
  }

  void Accept(LogRecord &action) override;

private:
  otio::SerializableObject::Retainer<otio::Timeline> timeline_;
  OTIOTracker tracker_ = OTIOTracker::NewTrackerFromRoot(this);
  LogAppender &appender_;
};

} // namespace estd

#endif // ESTD_MODEL_TIMELINE_HH
