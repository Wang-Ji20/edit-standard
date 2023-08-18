//===------------------------------------------===
// estd 2023
//
// Identification: include/string_utils.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef ESTD_STRING_UTILS_HH
#define ESTD_STRING_UTILS_HH

#include <string>

namespace estd {

struct StringUtils {

  [[nodiscard("We are not inplaced")]] static auto
  StripSpaceTabCRLF(const std::string &str) -> std::string {
    std::string result;
    for (auto c : str) {
      if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
        result.push_back(c);
      }
    }
    return result;
  }

  [[nodiscard("We are not amused")]] static auto
  EqualIgnoreSpaceTabCRLF(const std::string &lhs, const std::string &rhs)
      -> bool {
    return StripSpaceTabCRLF(lhs) == StripSpaceTabCRLF(rhs);
  }
};

} // namespace estd

#endif // ESTD_STRING_UTILS_HH
