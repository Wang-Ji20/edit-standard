//===------------------------------------------===
// estd 2023
//
// Identification: include/log_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_LOG_MANAGER_HH
#define ESTD_LOG_MANAGER_HH

#include "checkpointer.hh"
#include "log_appender.hh"
#include "log_record.hh"
#include "log_revoker.hh"
#include "log_rewriter.hh"
#include "snapshot.hh"

namespace estd {

class LogManager {
public:
  void
  Commit(const LogID &logid);

  void
  Rollback(const LogID &logid);

  void
  SnapShot(const LogID &logid);

  void
  CheckPoint(const LogID &logid);

  void
  Rewrite(const LogRange &range, LogRewriter &rewriter);

private:
  std::vector<LogRecord> logSink_;
};

} // namespace estd

#endif // ESTD_LOG_MANAGER_HH
