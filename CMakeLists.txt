cmake_minimum_required(VERSION 3.1)
project(Heart)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# Include directories
include_directories("src")

# Main executable
set(MAIN_EXECUTABLE "heart")
add_subdirectory(src)
add_executable(${MAIN_EXECUTABLE} ${SOURCE})

# Turn on C++11 support
set_property(TARGET ${MAIN_EXECUTABLE} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${MAIN_EXECUTABLE} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${TEST_EXECUTABLE} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${TEST_EXECUTABLE} PROPERTY CXX_STANDARD 11)

# Link libraries
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${MAIN_EXECUTABLE} ${SFML_LIBRARIES})
endif()

# Copy resources
file(COPY ${PROJECT_SOURCE_DIR}/read DESTINATION ${PROJECT_BINARY_DIR})
