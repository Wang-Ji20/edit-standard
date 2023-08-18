//===------------------------------------------===
// estd 2023
//
// Identification: include/model/abstract_model.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_ABSTRACT_MODEL_HH
#define ESTD_ABSTRACT_MODEL_HH

namespace estd {

struct LogRecord;

class AbstractModel {
public:
  virtual ~AbstractModel() = default;

  virtual void Accept(LogRecord &action) = 0;
};

} // namespace estd

#endif // ESTD_ABSTRACT_MODEL_HH
