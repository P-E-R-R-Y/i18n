# ----------- general -----------

cmake_minimum_required(VERSION 3.14)
project(i18n LANGUAGES CXX)

# Define the option -DCXX_STANDARD=11
set(CXX_STANDARD 20 CACHE STRING "Build with C++??")

set(LEGACY_STANDARDS 11 14 17)
set(MODERN_STANDARDS 20 23 26)

if(CXX_STANDARD IN_LIST LEGACY_STANDARDS)
    set(CXX11 true)
    set(CMAKE_CXX_STANDARD 11)
    set(COMMON_FLAGS -Wall -Wextra -Werror -std=c++11)
    set(CXX_PATH cxx11)
elseif(CXX_STANDARD IN_LIST MODERN_STANDARDS)
    set(CMAKE_CXX_STANDARD 20)
    set(COMMON_FLAGS -Wall -Wextra -Werror -std=c++20)
    set(CXX_PATH cxx20)
else()
  message(FATAL_ERROR "Unsuported -DCXX_STANDARD=${CXX_STANDARD}. supported: 11, 14, 17, 20, 23, 26.")
endif()
# Only set default if not explicitly passed

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/no_source.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# ----------- includes -----------

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/${CXX_PATH}
)

#----------- dependencies -----------

if(APPLE)
  # macOS frameworks
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreFoundation")
endif()

#----------- dev -----------

target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_FLAGS})

#----------- test -----------
enable_testing()

if(CXX11) ##Gtest cxx17 or higher required
  set(TEST_NAME ${PROJECT_NAME}_test_runner)
  file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/cxx11/Test*.cpp)
else()
  list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
  find_package(GoogleTest REQUIRED)

  set(TEST_NAME ${PROJECT_NAME}_tests)
  file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/cxx20/Test*.cpp)
  set(GTEST true)
endif()

add_executable(${TEST_NAME} ${TEST_SOURCES})

target_include_directories(${TEST_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/includes
)

target_link_libraries(${TEST_NAME} PRIVATE
  ${PROJECT_NAME}
  $<$<BOOL:${GTEST}>:gtest_main>
)

target_compile_options(${TEST_NAME} PRIVATE ${COMMON_FLAGS})

if(CXX11)
  add_test(NAME run_i18n_tests COMMAND ${TEST_NAME})
else()
  include(GoogleTest)
  gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endif()

unset(CXX_STANDARD CACHE)