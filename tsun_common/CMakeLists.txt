add_library(tsun_common src/mmap.cpp)
target_compile_features(tsun_common PUBLIC cxx_std_26)
target_include_directories(tsun_common PUBLIC inc/)

include(CheckSymbolExists)

# mmap tests
check_symbol_exists(mmap "sys/mman.h" TSUN_COMMON_HAVE_MMAP)

if (TSUN_COMMON_HAVE_MMAP)
    target_compile_definitions(tsun_common PRIVATE TSUN_COMMON_HAVE_MMAP_)
else()
    message(FATAL_ERROR "Platform not supported")
endif()

set(TSUN_TESTING_LIBRARIES tsun_common)
include(../cmake/testing.cmake)

tsun_add_test_file(cursor_test tests/cursor_test.cpp)
