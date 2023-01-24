include(GoogleTest)

file(GLOB_RECURSE BAKE_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")

foreach (bake_test_source ${BAKE_TEST_SOURCES})
    # Create a human readable name
    get_filename_component(bake_test_filename ${bake_test_source} NAME)
    string(REPLACE ".cpp" "" bake_test_name ${bake_test_filename})

    # Add the test target separately
    add_executable(${bake_test_name} EXCLUDE_FROM_ALL ${bake_test_source})

    gtest_discover_tests(${bake_test_name}
            EXTRA_ARGS
            --gtest_color=auto
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${bake_test_name}.xml
            --gtest_catch_exceptions=0
            DISCOVERY_TIMEOUT 120
            PROPERTIES
            TIMEOUT 120
            )

    target_link_libraries(${bake_test_name} bake GTest::gtest_main)

    # Set test target properties and dependencies
    set_target_properties(${bake_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${bake_test_name}
            )
endforeach ()
