//===------------------------------------------===
// estd 2023
//
// Identification: include/writer/ostream_writer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_OSTREAM_WRITER_HH
#define ESTD_OSTREAM_WRITER_HH

#include "writer.hh"

namespace estd {

class OStreamWriter : public Writer {
public:
  OStreamWriter(std::ostream &os, SerializerPrototype ser)
      : Writer(std::move(ser)), os_(os) {}

  auto
  OnChecksumHook(uint64_t checksum) -> void override {
    Writer::WriteInternal(checksum, WriteMode::Raw, ChecksumMode::NoChecksum);
  }

  ~OStreamWriter() override = default;

protected:
  auto
  WriteRaw(const uint8_t *begin, const uint8_t *end) -> void override {
    std::copy(begin, end, std::ostream_iterator<uint8_t>(os_));
  }

  std::ostream &os_;
};

} // namespace estd

#endif // ESTD_OSTREAM_WRITER_HH
