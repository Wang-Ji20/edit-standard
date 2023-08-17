//===------------------------------------------===
// estd 2023
//
// Identification: lib/serializer/serializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "serializer/serializer.hh"

namespace estd {

/// a vtable anchor
void Serializer::WriteValue(const string &value) { WriteValue(value.c_str()); }

} // namespace estd
