project(hikidashi-cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -DMY_DEBUG")

cmake_minimum_required(VERSION 3.20)
find_package(GTest CONFIG REQUIRED)

add_executable(bisect_test tests/bisect_test.cpp)
target_link_libraries(bisect_test PRIVATE GTest::gtest GTest::gmock GTest::gtest_main)

add_executable(compress_test tests/compress_test.cpp)
target_link_libraries(compress_test PRIVATE GTest::gtest GTest::gmock GTest::gtest_main)

add_executable(rolling_hash_test tests/rolling_hash_test.cpp)
target_link_libraries(rolling_hash_test PRIVATE GTest::gtest GTest::gmock GTest::gtest_main)

add_executable(wavelet_matrix_test tests/wavelet_matrix_test.cpp)
target_link_libraries(wavelet_matrix_test PRIVATE GTest::gtest GTest::gmock GTest::gtest_main)
