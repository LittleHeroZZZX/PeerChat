set(NETWORK_SOURCES
    tcp_peer.cc)

add_library(network SHARED ${NETWORK_SOURCES})
target_include_directories(network PRIVATE ${PROJECT_SOURCE_DIR}/include)