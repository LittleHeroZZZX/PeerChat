add_executable(PeerChat test.cc)
target_link_libraries(PeerChat PRIVATE logger)
target_link_libraries(PeerChat PRIVATE network)
target_link_libraries(PeerChat PRIVATE message)

target_include_directories(PeerChat PRIVATE ${CMAKE_SOURCE_DIR}/include/common)
