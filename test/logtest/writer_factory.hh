//===------------------------------------------===
// estd 2023
//
// Identification: test/logtest/writer_factory.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_TEST_WRITER_FACTORY_HH
#define ESTD_TEST_WRITER_FACTORY_HH

#include "serializer/json_serializer.hh"
#include "writer/ostream_writer.hh"
#include <fstream>
#include <iostream>
#include <tuple>

namespace estd {

class WriterFactory {
public:
  static auto MakeConsoleWriter() -> std::unique_ptr<Writer> {
    return std::make_unique<OStreamWriter>(
        std::cerr, []() { return std::make_unique<JsonSerializer>(); });
  }

  static auto MakeFileWriter(std::string path)
      -> std::tuple<std::unique_ptr<Writer>, std::unique_ptr<std::fstream>> {
    auto file =
        std::make_unique<std::fstream>(path, std::ios::out | std::ios::binary);
    auto fileWriter = std::make_unique<OStreamWriter>(
        *file, []() { return std::make_unique<JsonSerializer>(); });
    return std::make_tuple(std::move(fileWriter), std::move(file));
  }
};

} // namespace estd

#endif // ESTD_TEST_WRITER_FACTORY_HH
