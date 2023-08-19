//===------------------------------------------===
// estd 2023
//
// Identification: include/model/clip.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_MODEL_CLIP_HH
#define ESTD_MODEL_CLIP_HH

#include "abstract_model.hh"
#include "opentimelineio/clip.h"

namespace estd {

namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

class Clip : public AbstractModel {
private:
  std::unique_ptr<otio::Clip> clip_;
};

} // namespace estd

#endif // ESTD_MODEL_CLIP_HH
