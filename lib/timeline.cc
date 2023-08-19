//===------------------------------------------===
// estd 2023
//
// Identification: lib/timeline.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "model/timeline.hh"
#include "memory.hh"

namespace estd {

struct TimelineModifier {
  otio::RationalTime oldGlobalStartTime;
  otio::RationalTime newGlobalStartTime;

  static auto
  Deserialize(const std::vector<uint8_t> &data) -> TimelineModifier {
    if (data.size() != sizeof(TimelineModifier)) {
      throw std::runtime_error("Invalid data size for timeline modify");
    }
    TimelineModifier modifier;
    auto datap = data.data();
    modifier.oldGlobalStartTime =
        otio::RationalTime(MemoryUtils::Load<double>(datap),
                           MemoryUtils::Load<double>(datap, sizeof(double)));
    modifier.newGlobalStartTime = otio::RationalTime(
        MemoryUtils::Load<double>(datap, sizeof(double) * 2),
        MemoryUtils::Load<double>(datap, sizeof(double) * 3));
    return modifier;
  }
};

struct TimelineModifyExecutor {
public:
  TimelineModifyExecutor(const LogRecord &log, Timeline &onEffectTimeline) {
    Verify(log);
    Execute(log.data, onEffectTimeline);
  }

private:
  void
  Verify(const LogRecord &log) {
    if (log.type != LogType::kModify) {
      throw std::runtime_error("Invalid log type for timeline modify");
    }
  }

  void
  Execute(const std::vector<uint8_t> &data, Timeline &onEffectTimeline) {
    TimelineModifier modifier = TimelineModifier::Deserialize(data);
    onEffectTimeline.timeline_->set_global_start_time(
        modifier.newGlobalStartTime);
  }

  enum class Field { kGlobalStartTime };
};

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

void
Timeline::Accept(LogRecord &action) {
  switch (action.type) {
  case LogType::kModify:
    TimelineModifyExecutor(action, *this);
    break;
  default:
    throw std::runtime_error("Unsupported log type for timeline");
  }
}

void
Timeline::SetGlobalStartTime(const opentime::RationalTime &time) {
  TimelineModifier modifier;
  modifier.oldGlobalStartTime = timeline_->global_start_time().value();
  modifier.newGlobalStartTime = time;
  std::vector<uint8_t> data(sizeof(TimelineModifier));
  MemoryUtils::Store(data.data(), modifier);
  appender_.Append(LogType::kModify, tracker_.GetLocation(), data);
}

} // namespace estd
