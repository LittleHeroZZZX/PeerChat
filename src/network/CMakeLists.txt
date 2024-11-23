set(NETWORK_SOURCES tcp_peer.cc udp_peer.cc common_net.cc)

add_library(network SHARED ${NETWORK_SOURCES})
target_include_directories(network PUBLIC ${PROJECT_SOURCE_DIR}/include/network)
target_include_directories(network PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(network PUBLIC logger)
target_link_libraries(network PUBLIC message)
