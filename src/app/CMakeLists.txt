project(PeerChat LANGUAGES CXX)

# 查找所需的 Qt 组件
find_package(Qt6 REQUIRED COMPONENTS Core Quick Network)

# 设置资源文件路径
set(QML_RESOURCES ${CMAKE_SOURCE_DIR}/resources/resources.qrc)

# 设置源文件
set(APP_SOURCES network_mgr.cc peer_chat.cc
                ${CMAKE_SOURCE_DIR}/include/app/network_mgr.h)

# 使用 qt_add_executable，只添加一次资源文件
qt_add_executable(PeerChat ${APP_SOURCES} ${QML_RESOURCES})

qt_add_qml_module(
  PeerChat
  URI
  PeerChat
  VERSION
  1.0
  SOURCES
  network_mgr.cc
  ${CMAKE_SOURCE_DIR}/include/app/network_mgr.h)

# 包含目录
target_include_directories(
  PeerChat
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include/app
          ${CMAKE_SOURCE_DIR}/src/network ${CMAKE_SOURCE_DIR}/src/common
          ${CMAKE_SOURCE_DIR}/src/utils)

# 链接库
target_link_libraries(PeerChat PRIVATE Qt6::Core Qt6::Quick network logger
                                       message)

# 拷贝可执行文件到build目录
add_custom_command(
  TARGET PeerChat
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PeerChat>
          ${CMAKE_BINARY_DIR}/bin/PeerChat)
