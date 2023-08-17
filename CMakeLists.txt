CMAKE_MINIMUM_REQUIRED(VERSION 3.19)

project(
  edit-standard
  VERSION
  0.0.0  
)

# no inplace build no
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  if(NOT DEFINED WITH_IN_SOURCE_BUILD)
    message(FATAL_ERROR
      "CMake generation for estd is not allowed within the source directory!"
      "\n Remove \"${CMAKE_SOURCE_DIR}/CMakeCache.txt\" and try again from another folder, e.g.:"
      "\n "
      "\n rm -rf CMakeCache.txt CMakeFiles"
      "\n cd .."
      "\n mkdir cmake-make"
      "\n cd cmake-make"
      "\n cmake ../estd"
      "\n "
      "\n Alternately define WITH_IN_SOURCE_BUILD to force this option (not recommended!)"
    )
  endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter -Wno-attributes")

# emit compiler settings.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# if define TSAN, use thread sanitizer
if(SANITIZER STREQUAL "TSAN")
    message(STATUS "Using TSAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer")
else ()
    message(STATUS "Using ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include
set(EDITSTD_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(
  EDITSTD_THIRD_PARTY_INCLUDE_DIRECTORIES
  ${PROJECT_SOURCE_DIR}/third_party/otio/src
  ${PROJECT_SOURCE_DIR}/third_party/yyjson/src
)
include_directories(${EDITSTD_SRC_INCLUDE_DIR} ${EDITSTD_THIRD_PARTY_INCLUDE_DIRECTORIES})

# source codes
add_subdirectory(lib)
add_subdirectory(third_party)

# if this project is included by other project, don't build test
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
