cmake_minimum_required(VERSION 3.14)
project(tsun)

enable_testing()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

option(TSUN_USE_LOCAL_GTEST "Uses local GTest" OFF)

if(TSUN_USE_LOCAL_GTEST)
  find_package(GTest REQUIRED)
else()
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(tsun_common/) 
add_subdirectory(tsunc_frontend/)
add_subdirectory(tsunc/) 
