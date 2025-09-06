cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

set(COPENPUBLISH_VERSION_MAJOR "0")
set(COPENPUBLISH_VERSION_MINOR "1")
set(COPENPUBLISH_VERSION_PATCH "0")
set(COPENPUBLISH_VERSION ${COPENPUBLISH_VERSION_MAJOR}.${COPENPUBLISH_VERSION_MINOR}.${COPENPUBLISH_VERSION_PATCH})

project(copenpubilsh VERSION ${COPENPUBLISH_VERSION} LANGUAGES C)

# --------------------------------------------------------------------------------
# Build Options
# --------------------------------------------------------------------------------
option(CODE_COVERAGE "Enable coverage reporting" OFF)

# --------------------------------------------------------------------------------
# Do the build
# --------------------------------------------------------------------------------
set(UNIT_TESTING OFF)

add_subdirectory(corrosion)

if (NOT DEFINED COPENPUBLISH_SOURCE_DIR OR COPENPUBLISH_SOURCE_DIR MATCHES "")
set(COPENPUBLISH_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
endif()

corrosion_import_crate(MANIFEST_PATH "${COPENPUBLISH_SOURCE_DIR}/Cargo.toml")

if (BUILD_COPENPUBLISH_TESTS)
    enable_testing()
    add_subdirectory(cmocka)
    add_subdirectory(tests)
endif()
