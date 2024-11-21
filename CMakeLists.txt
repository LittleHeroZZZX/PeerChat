cmake_minimum_required(VERSION 3.5)
project(PeerChat LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/network)
aux_source_directory(./src srcs)

add_executable(${PROJECT_NAME}
    ${srcs} 
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

