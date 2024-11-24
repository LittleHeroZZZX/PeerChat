#include <netinet/in.h>
#include <string>
#include "common/peer_message.h"
class NetworkPeer {
  public:
    virtual ~NetworkPeer() {}

    // 初始化本机套接字
    virtual bool initialize(const in_port_t port) = 0;

    // 异步发送消息
    virtual bool send_msg_async(const Message &msg) = 0;

    // 接收消息
    virtual Message receive_msg() = 0;

    virtual void set_on_receive_msg(std::function<void()> on_receive_msg) = 0;

    // 关闭连接
    virtual void close_connection() = 0;

    // 检查是否已连接
    virtual bool is_connected() const = 0;
};
