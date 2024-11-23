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
    }

    UDPPeer(const in_port_t port) : local_port(port) {
        sender_thread = std::thread(&UDPPeer::sender_thread_func, this);
    }

    ~UDPPeer() override {
        log_error("enter ~UDPPeer()");
        stop = true;
        cv.notify_all();
        if (sender_thread.joinable()) {
            sender_thread.join();
        }
        close_connection();
    }

    UDPPeer(const UDPPeer &) = delete;
    UDPPeer &operator=(const UDPPeer &) = delete;

    // 创建socket -> 绑定端口
    bool initialize(const in_port_t port) override;
    bool initialize();
    bool set_remote(const std::string &ip, const in_port_t port);
    virtual bool send_msg_async(const Message &msg) override;
    std::string receive_msg() override;
    void close_connection() override;
    bool is_connected() const override { return connected; };

  private:
    std::optional<int> socket_fd;
    bool connected = false;

    std::optional<in_port_t> local_port;
    std::optional<sockaddr> remote_addr;
    std::optional<socklen_t> addr_len;

    std::queue<std::string> msg_queue;
    std::mutex queue_mutex;
    std::condition_variable cv;
    std::atomic<bool> stop = false;
    std::thread sender_thread;

    void close_socket();
    void sender_thread_func();
    bool send_msg(const std::string &msg);
};
