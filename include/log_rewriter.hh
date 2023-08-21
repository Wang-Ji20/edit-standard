//===------------------------------------------===
// estd 2023
//
// Identification: include/log_rewriter.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_LOG_REWRITER_HH
#define ESTD_LOG_REWRITER_HH

#include "log_record.hh"

namespace estd {

class LogRewriter {
public:
  LogRewriter() = default;
  ~LogRewriter() = default;

  virtual void
  PassRewrite(const LogRange& range) = 0;

};

  } // namespace estd

#endif // ESTD_LOG_REWRITER_HH
