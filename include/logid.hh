//===------------------------------------------===
// estd 2023
//
// Identification: include/logid.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_UUID_HH
#define ESTD_UUID_HH

#include <array>
#include <ctime>

namespace estd {

using LogID = std::array<char, 16>;

class LogIDGenerator {
public:
  LogIDGenerator() {
    std::srand(std::time(nullptr));
    seed_ = std::rand();
  }

  auto Generate() -> LogID {
    LogID id;
    std::memcpy(id.data(), &seed_, sizeof(seed_));
    std::memcpy(id.data() + sizeof(seed_), &counter_, sizeof(counter_));
    return id;
  }

private:
  u_int64_t seed_;
  u_int64_t counter_{};
};

} // namespace estd

#endif
