include(GoogleTest)

function(tsun_add_test_file test_name)
  add_executable(${test_name} ${ARGN})
  target_link_libraries(${test_name} ${TSUN_TESTING_LIBRARIES}
                        GTest::gtest_main)

  target_compile_features(${test_name} PRIVATE cxx_std_26)

  gtest_discover_tests(${test_name})
endfunction()
