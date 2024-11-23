cmake_minimum_required(VERSION 3.5)
project(PeerChat LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
  Qt6
  COMPONENTS Widgets
  REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/network)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/utils)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/common)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party)
