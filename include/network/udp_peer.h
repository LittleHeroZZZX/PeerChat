#pragma once
#include <unistd.h>
#include <condition_variable>
#include <optional>
#include <queue>
#include <string>
#include "logger.h"
#include "network_peer.h"

class UDPPeer : public NetworkPeer {
  public:
    UDPPeer() {
        sender_thread = std::thread(&UDPPeer::sender_thread_func, this);
        receiver_thread = std::thread(&UDPPeer::receiver_thread_func, this);
    }

    UDPPeer(const in_port_t port) : local_port(port) {
        sender_thread = std::thread(&UDPPeer::sender_thread_func, this);
        receiver_thread = std::thread(&UDPPeer::receiver_thread_func, this);
    }

    ~UDPPeer() override {
        stop = true;
        cv.notify_all();
        if (sender_thread.joinable()) {
            sender_thread.join();
        }
        if (receiver_thread.joinable()) {
            receiver_thread.join();
        }
        close_connection();
    }

    UDPPeer(const UDPPeer &) = delete;
    UDPPeer &operator=(const UDPPeer &) = delete;

    // 创建socket -> 绑定端口
    bool initialize(const in_port_t port) override;
    bool initialize();
    bool initialized() const { return socket_fd.has_value(); }
    bool set_remote(const std::string &ip, const in_port_t port);
    bool send_msg_async(const Message &msg) override;
    Message receive_msg() override;
    void close_connection() override;
    bool is_connected() const override { return connected; };
    void set_on_receive_msg(std::function<void()> on_receive_msg) override;
    in_port_t get_local_port() const { return local_port.value_or(0); }

  private:
    std::optional<int> socket_fd;
    bool connected = false;

    std::optional<in_port_t> local_port;
    std::optional<sockaddr> remote_addr;
    std::optional<socklen_t> addr_len;

    std::queue<std::string> send_msg_queue;
    std::queue<std::string> recv_msg_queue;
    std::mutex send_queue_mutex;
    std::mutex recv_queue_mutex;
    std::condition_variable cv;
    std::atomic<bool> stop = false;
    std::thread sender_thread;
    std::thread receiver_thread;

    std::optional<std::function<void()>> notify_receive_msg;

    void close_socket();
    void sender_thread_func();
    void receiver_thread_func();

    // 发送和接收数据，这是最底层接口，发送的序列化后的序列化数据
    bool send_msg(const std::string &msg);
    bool recv_msg(std::string &msg);
};
