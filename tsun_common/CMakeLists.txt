add_library(tsun_common src/fixcomp.cpp)
target_compile_features(tsun_common PUBLIC cxx_std_26)
target_include_directories(tsun_common PUBLIC inc/)

set(TSUN_TESTING_LIBRARIES tsun_common)
include(../cmake/testing.cmake)

tsun_add_test_file(cursor_test tests/cursor_test.cpp)
