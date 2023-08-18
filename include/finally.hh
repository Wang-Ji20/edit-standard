//===------------------------------------------===
// estd 2023
//
// Identification: include/control/finally.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------------------------------===
// modified a lot based on C++ core guidelines.
//===------------------------------------------------------------------------===

#ifndef ESTD_CONTROL_FINALLY_HH
#define ESTD_CONTROL_FINALLY_HH

namespace estd {

template <class F> class FinalAction {
public:
  template <class FN>
  [[nodiscard("temp obj will be discarded right away. no deferred "
              "effect.")]] auto static constexpr finally(FN func) {
    return FinalAction(func);
  }
  ~FinalAction() { act(); }

  // disallow copy, allow move (extend lifetime)
  FinalAction(const FinalAction &) = delete;
  auto operator=(const FinalAction &) -> FinalAction & = delete;

  FinalAction(FinalAction &&) = default;
  auto operator=(FinalAction &&) -> FinalAction & = default;

private:
  // To avoid unnamed temporary object.
  explicit FinalAction(F func) : act(func) {}
  F act;
};

} // namespace estd

#endif // ESTD_CONTROL_FINALLY_HH
