include(GoogleTest)

include_directories(${SOURCE_DIRECTORY})

add_executable(tests
  test_quaternions.cpp
  main.cpp)

gtest_discover_tests(tests)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
