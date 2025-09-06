cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

set(CPPOPENPUBLISH_VERSION_MAJOR "0")
set(CPPOPENPUBLISH_VERSION_MINOR "1")
set(CPPOPENPUBLISH_VERSION_PATCH "0")
set(CPPOPENPUBLISH_VERSION
    ${CPPOPENPUBLISH_VERSION_MAJOR}.${CPPOPENPUBLISH_VERSION_MINOR}.${CPPOPENPUBLISH_VERSION_PATCH}
)

project(
  cppopenpublish
  VERSION ${CPPOPENPUBLISH_VERSION}
  LANGUAGES CXX)

# --------------------------------------------------------------------------------
# Build Options
# --------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------
# Do the build
# --------------------------------------------------------------------------------
set(PUBLISH_HEADERS
    ${PROJECT_SOURCE_DIR}/include/openpublish/context_iter.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/context_view.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/context.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/core.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/publish.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/runner.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/status.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/value_iter.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/value_view.h
    ${PROJECT_SOURCE_DIR}/include/openpublish/value.h)
set(PUBLISH_SRC
    ${PROJECT_SOURCE_DIR}/src/context_iter.cpp
    ${PROJECT_SOURCE_DIR}/src/context_view.cpp
    ${PROJECT_SOURCE_DIR}/src/context.cpp
    ${PROJECT_SOURCE_DIR}/src/core.cpp
    ${PROJECT_SOURCE_DIR}/src/publish.cpp
    ${PROJECT_SOURCE_DIR}/src/runner.cpp
    ${PROJECT_SOURCE_DIR}/src/status.cpp
    ${PROJECT_SOURCE_DIR}/src/value_iter.cpp
    ${PROJECT_SOURCE_DIR}/src/value_view.cpp
    ${PROJECT_SOURCE_DIR}/src/value.cpp)

add_library(${CMAKE_PROJECT_NAME} ${PUBLISH_HEADERS} ${PUBLISH_SRC})

set(COPENPUBLISH_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../c)
add_subdirectory(${PROJECT_SOURCE_DIR}/../c ${CMAKE_BINARY_DIR}/copenpublish)

target_include_directories(
  ${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include
                               ${PROJECT_SOURCE_DIR}/../c/include)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${CMAKE_PROJECT_NAME} copenpublish)

install(FILES ${PUBLISH_HEADERS} DESTINATION include/openpublish)
install(
  TARGETS ${CMAKE_PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  DESTINATION lib)

if(BUILD_CPPOPENPUBLISH_TESTS)
  enable_testing()
  add_subdirectory(googletest EXCLUDE_FROM_ALL)
  # For Windows: Prevent overriding the parent project's compiler/linker
  # settings
  set(gtest_force_shared_crt
      ON
      CACHE BOOL "" FORCE)
  add_subdirectory(tests)
endif()
