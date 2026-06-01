add_library(tsunc_frontend src/lexer.cpp src/token.cpp)
target_compile_features(tsunc_frontend PUBLIC cxx_std_26)
target_include_directories(tsunc_frontend PUBLIC inc/)
target_link_libraries(tsunc_frontend PUBLIC tsun_common)

set(TSUN_TESTING_LIBRARIES tsun_common tsunc_frontend)
include(../cmake/testing.cmake)

tsun_add_test_file(lexer_test tests/lexer_test.cpp)
