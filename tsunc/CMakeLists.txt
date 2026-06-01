add_executable(tsunc src/main.cpp)
target_compile_features(tsunc PRIVATE cxx_std_26)
target_include_directories(tsunc PUBLIC inc/)
target_link_libraries(tsunc PUBLIC tsun_common tsunc_frontend)

include(../cmake/testing.cmake)
