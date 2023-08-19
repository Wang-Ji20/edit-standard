//===------------------------------------------===
// estd 2023
//
// Identification: include/otio_tracker.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_OTIO_TRACKER_HH
#define ESTD_OTIO_TRACKER_HH

//===------------------------------------------------------------------------===
// This class tracks which OTIO object is being modified.
// should always PASS BY VALUE
//===------------------------------------------------------------------------===

#include "log_record.hh"

namespace estd {

class OTIOTracker {
public:
  [[nodiscard]] static auto
  NewTrackerFromRoot(AbstractModel &as) -> OTIOTracker {
    return OTIOTracker(as);
  }

  [[nodiscard]] auto
  GetLocation() const -> const OtioLocation & {
    return location_;
  }

  [[nodiscard]] auto
  ArraySub(size_t idx, OTIOType ty, AbstractModel &as) -> OTIOTracker {
    return {*this, "[" + std::to_string(idx) + "]", ty, as};
  }

  [[nodiscard]] auto
  DictSub(std::string idx, OTIOType ty, AbstractModel &as) -> OTIOTracker {
    return {*this, "[" + idx + "]", ty, as};
  }

  [[nodiscard]] auto
  FieldRef(std::string field, OTIOType ty, AbstractModel &as) -> OTIOTracker {
    return {*this, "." + field, ty, as};
  }

private:
  explicit OTIOTracker(AbstractModel &as)
      : location_("$", OTIOType::kTimeline, as) {}
  OTIOTracker(const OTIOTracker &parent,
              std::string child,
              OTIOType childTy,
              AbstractModel &as)
      : location_(parent.location_.rawLocation + child, childTy, as) {}

  OtioLocation location_;
};

} // namespace estd

#endif // ESTD_OTIO_TRACKER_HH
