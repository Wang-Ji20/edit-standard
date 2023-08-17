//===------------------------------------------===
// estd 2023
//
// Identification: include/logid.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_LOGID_HH
#define ESTD_LOGID_HH

#include <array>
#include <ctime>

namespace estd {

struct LogID {
  auto data() -> uint8_t * { return id_.data(); }
  template <typename Ser>
  friend void estdWriteValue(Ser &serializer, const LogID &value) {
    serializer.OnVectorBegin(16);
    for (auto c : value.id_) {
      serializer.WriteValue(c);
    }
    serializer.OnVectorEnd(16);
  }

  std::array<uint8_t, 16> id_;
};

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

#endif // ESTD_LOGID_HH
