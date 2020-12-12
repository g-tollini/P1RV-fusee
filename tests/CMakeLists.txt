set(BINARY unit_tests)

add_executable(
    ${BINARY}
    tests.cpp)

target_link_libraries(${BINARY} ${GTEST_LIBRARY} pthread)