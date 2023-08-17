//===------------------------------------------===
// estd 2023
//
// Identification: include/serializer/serializer_type_traits.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include <memory>

#include <unordered_set>
#include <vector>

#include <type_traits>

namespace estd {

using std::shared_ptr;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

class Deserializer;

/// we can do this because Substitution Failure Is Not An Error (SFINAE)
template <typename T, typename = T> struct has_deserialize : std::false_type {};

// clang-format off
template <typename T>
struct has_deserialize<
    T,
    typename std::enable_if<std::is_same<decltype(T::estdReadValue),
                                         unique_ptr<T>(Deserializer &) // function unique_ptr<T> functionName(Deserializer&);
                                        >::value,
                          T>::type
                      > : std::true_type {};

template <typename T>
struct has_deserialize<
    T,
    typename std::enable_if<std::is_same<decltype(T::estdReadValue),
                                         shared_ptr<T>(Deserializer &)
                                        >::value,
                            T
                           >::type
                      > : std::true_type {};

template <typename T>
struct has_deserialize<
    T,
    typename std::enable_if<
        std::is_same<decltype(T::estdReadValue), T(Deserializer &)>::value,
                          T>::type
                      > : std::true_type {};

// clang-format on

template <typename T> struct is_deserializable : has_deserialize<T> {};

//===------------------------------------------------------------------------===
// Pointers. We allocate heap memory, and return a pointer to it.
//===------------------------------------------------------------------------===

template <typename T> struct is_unique_ptr : std::false_type {};

template <typename T>
struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {
  using ELEMENT_TYPE = T;
};

template <typename T> struct is_shared_ptr : std::false_type {};

template <typename T> struct is_shared_ptr<shared_ptr<T>> : std::true_type {
  using ELEMENT_TYPE = T;
};

//===------------------------------------------------------------------------===
// Containers.
//===------------------------------------------------------------------------===

template <typename T> struct is_pair : std::false_type {};
template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {
  using FIRST_TYPE = T;
  using SECOND_TYPE = U;
};

template <typename T> struct is_unordered_set : std::false_type {};
template <typename... Args>
struct is_unordered_set<std::unordered_set<Args...>> : std::true_type {
  using ELEMENT_TYPE = typename std::unordered_set<Args...>::value_type;
};

template <typename T> struct is_vector : std::false_type {};
template <typename T>
struct is_vector<typename estd::vector<T>> : std::true_type {
  using ELEMENT_TYPE = T;
};

template <typename T> struct is_unordered_map : std::false_type {};
template <typename... Args>
struct is_unordered_map<std::unordered_map<Args...>> : std::true_type {
  using KEY_TYPE = typename std::tuple_element<0, std::tuple<Args...>>::type;
  using VALUE_TYPE = typename std::tuple_element<1, std::tuple<Args...>>::type;
  using HASH_TYPE = typename std::tuple_element<2, std::tuple<Args...>>::type;
  using EQUAL_TYPE = typename std::tuple_element<3, std::tuple<Args...>>::type;
};

} // namespace estd
