//===------------------------------------------===
// estd 2023
//
// Identification: include/model/respository.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_MODEL_REPOSITORY_HH
#define ESTD_MODEL_REPOSITORY_HH

#include "abstract_model.hh"
#include "log_appender.hh"
#include "model/timeline.hh"
#include "otio_tracker.hh"
#include <memory>

namespace estd {

class Respository : public AbstractModel {
public:
  Respository(Writer &logWriter)
      : appender_(std::make_unique<LogAppender>(logWriter)) {}

  ~Respository() override = default;

  template <typename Ser>
  friend void
  estdWriteValue(Ser &serializer, const Respository &value) {
    serializer.WriteValue(*value.timeline_);
  }

  void
  AddNewTimeline();

  inline auto
  GetTimeline() -> Timeline & {
    return *timeline_;
  }

  void
  Accept(LogRecord &action) override;

private:
  auto
  Verify(const LogRecord &action) -> bool;

  std::unique_ptr<Timeline> timeline_;
  std::unique_ptr<LogAppender> appender_;

  // location is invalid at this level, so we use a dummy location
  // to keep api consistency.
  OtioLocation dummyLoc{"$", OTIOType::kDummy, *this};
  std::vector<uint8_t> dummyVec;
};

} // namespace estd

#endif // ESTD_MODEL_REPOSITORY_HH
