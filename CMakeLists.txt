# ----------- general -----------

cmake_minimum_required(VERSION 3.24)
project(i18n LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/no_source.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# ----------- includes -----------

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

#----------- dependencies -----------

if(APPLE)
  # macOS frameworks
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreFoundation")
endif()

#----------- dev -----------

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

#----------- test -----------
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(GoogleTest REQUIRED)

enable_testing()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME}_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/tests/includes
)

target_link_libraries(${PROJECT_NAME}_tests 
	gtest_main
	${PROJECT_NAME}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
