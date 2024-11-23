add_library(message SHARED peer_message.cc)
target_include_directories(message PUBLIC ${CMAKE_SOURCE_DIR}/include/common)
target_include_directories(
  message PUBLIC ${CMAKE_SOURCE_DIR}/third_party/cereal/include)
