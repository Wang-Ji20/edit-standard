//===------------------------------------------===
// estd 2023
//
// Identification: lib/model/respository.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "model/respository.hh"
#include "model/timeline.hh"
#include "otio_tracker.hh"

namespace estd {

void Respository::AddNewTimeline() {
  appender_->Append(LogType::kCreate, dummyLoc, dummyVec);
}

auto Respository::Verify(const LogRecord &action) -> bool {
  return action.type == LogType::kCreate && action.data == dummyVec &&
         action.location == dummyLoc;
}

void Respository::Accept(LogRecord &action) {
  if (!Verify(action)) {
    throw std::invalid_argument("invalid action fot Respository");
  }
  timeline_ = std::make_unique<Timeline>(*appender_);
}

} // namespace estd
