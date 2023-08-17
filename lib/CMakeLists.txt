add_subdirectory(serializer)

set(
    ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES}
    log_record.cc
)

add_library(
    estd
    STATIC
    ${ALL_OBJECT_FILES}
)

set(
    THIRD_PARTY_DEPENDENCIES
    opentimelineio
    yyjson
)

target_link_libraries(
    estd
    PUBLIC
    ${THIRD_PARTY_DEPENDENCIES}
)

target_include_directories(
  estd PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
