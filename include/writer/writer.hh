//===------------------------------------------===
// estd 2023
//
// Identification: include/writer/writer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------------------------------===
// class Writer writes serialized data to a certain destination with integrity.
//===------------------------------------------------------------------------===

#ifndef ESTD_LOG_WRITER_HH
#define ESTD_LOG_WRITER_HH

#include "hash.hh"
#include "serializer/serializer.hh"
#include <memory>
#include <utility>

namespace estd {

using SerializerPrototype = std::function<std::unique_ptr<Serializer>(void)>;

class Writer {
public:
  Writer() = default;

  Writer(SerializerPrototype prototype) : prototype_(std::move(prototype)) {}

  virtual ~Writer() = default;

  // most of the time client should use this.
  template <typename T> auto Write(const T &t) -> void { WriteInternal(t); }

protected:
  virtual auto OnChecksumHook(uint64_t checksum) -> void = 0;
  virtual auto WriteRaw(const uint8_t *begin, const uint8_t *end) -> void = 0;

  enum class WriteMode { Serialize, Raw };
  enum class ChecksumMode { Checksum, NoChecksum };

  // this function acts as a dispatcher. syntax sugar, maybe a bad idea really.
  template <typename T>
  auto WriteInternal(const T &t, WriteMode serialize = WriteMode::Serialize,
                     ChecksumMode checksum = ChecksumMode::Checksum) -> void {
    auto begin = reinterpret_cast<const uint8_t *>(&t);
    auto end = reinterpret_cast<const uint8_t *>(&t + 1);
    if (checksum == ChecksumMode::Checksum) {
      Checksum(begin, end);
    }
    if (serialize == WriteMode::Serialize) {
      WriteSerialized(t);
      return;
    }
    WriteRaw(begin, end);
  }

  SerializerPrototype prototype_ = []() { return nullptr; };

private:
  template <typename T> auto WriteSerialized(const T &t) -> void {
    auto serializer = prototype_();
    if (!serializer) {
      throw std::runtime_error("Serializer prototype is not set.");
    }
    serializer->WriteValue(t);
    auto v = serializer->Serialize();
    WriteRaw(v.begin().base(), v.end().base());
    return;
  }

  auto Checksum(const uint8_t *begin, const uint8_t *end) -> void {
    auto checksum = HashingUtilities::Checksum(begin, end - begin);
    OnChecksumHook(checksum);
  }
};

} // namespace estd

#endif // ESTD_LOG_WRITER_HH
