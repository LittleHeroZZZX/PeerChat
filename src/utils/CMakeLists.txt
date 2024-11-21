add_library(logger SHARED logger.cc)

target_link_libraries(logger PUBLIC spdlog)
target_include_directories(logger PUBLIC ${CMAKE_SOURCE_DIR}/include/utils)
