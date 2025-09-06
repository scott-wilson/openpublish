set(TESTS test_context test_runner test_status test_value)
set(TEST_HEADERS)

if(BUILD_WITH_CHECK_COVERAGE)
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

foreach(TEST ${TESTS})
  add_executable(${TEST} ${TEST}.cpp ${TEST_HEADERS})
  target_include_directories(
    ${TEST}
    PRIVATE "${PROJECT_SOURCE_DIR}/tests"
            "${PROJECT_SOURCE_DIR}/tests/common"
            "${PROJECT_SOURCE_DIR}/include"
            "${PROJECT_SOURCE_DIR}/../c/include"
            "${PROJECT_SOURCE_DIR}/googletest/googletest/include")
  target_link_libraries(${TEST} PUBLIC gtest_main copenpublish cppopenpublish)
  add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()
