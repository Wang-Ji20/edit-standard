include_directories(${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include)

include(GoogleTest)

add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB_RECURSE ESTD_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cc")

message(STATUS "Found ${ESTD_TEST_SOURCES} test sources: ${PROJECT_SOURCE_DIR}/test/")

foreach (estd_test_source ${ESTD_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(estd_test_filename ${estd_test_source} NAME)
    string(REPLACE ".cc" "" estd_test_name ${estd_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${estd_test_name} EXCLUDE_FROM_ALL ${estd_test_source})
    add_dependencies(build-tests ${estd_test_name})
    add_dependencies(check-tests ${estd_test_name})

    gtest_discover_tests(${estd_test_name}
            EXTRA_ARGS
            --gtest_color=auto
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${estd_test_name}.xml
            --gtest_catch_exceptions=0
            DISCOVERY_TIMEOUT 120
            PROPERTIES
            TIMEOUT 120
            )

    target_link_libraries(${estd_test_name} estd gtest gmock_main)

    # Set test target properties and dependencies.
    set_target_properties(${estd_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${estd_test_name}
            )
endforeach ()
