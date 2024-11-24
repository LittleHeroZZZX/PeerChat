#include "udp_peer.h"
#include <netinet/in.h>
#include <unistd.h>
#include <format>
#include "common_net.h"
#include "logger.h"

bool UDPPeer::initialize(const in_port_t port) {
    if (local_port.has_value()) {
        log_error(std::format("Try to REBIND port to socket <{}>",
                              socket_fd.value()));
        return false;
    }
    local_port = port;
    return initialize();
}

bool UDPPeer::initialize() {
    if (socket_fd.has_value()) {
        log_error(std::format(
            "This UDPPeer has already been initialized! The socket fd is :{}",
            socket_fd.value()));
        return false;
    }

    if (!local_port.has_value()) {
        log_error("This UDPPeer has NO port number!");
        return false;
    }

    // create socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        log_warning("Fail to create socket.");
        return false;
    }

    // set address
    struct sockaddr_in server_addr = {};  //  set all members to 0
    socklen_t addr_len = sizeof(server_addr);
    fill_address(std::nullopt, local_port.value(), server_addr);

    // bind socket
    int bind_ret = bind(socket_fd.value(),
                        reinterpret_cast<sockaddr *>(&server_addr),
                        addr_len);
    if (bind_ret < 0) {
        log_warning(std::format("Fail to bind port <{}>", local_port.value()));
        close_socket();
        return false;
    }
    return true;
}

bool UDPPeer::set_remote(const std::string &ip, const in_port_t port) {
    if (remote_addr.has_value() || addr_len.has_value()) {
        log_error("This UDPPeer has already set remote address!");
        return false;
    }

    struct sockaddr_in remote = {};
    addr_len = sizeof(remote);
    fill_address(ip, port, remote);
    remote_addr = *reinterpret_cast<sockaddr *>(&remote);
    return true;
}

bool UDPPeer::send_msg_async(const Message &msg) {
    {
        std::lock_guard<std::mutex> lock(send_queue_mutex);
        send_msg_queue.emplace(std::move(msg.serialize()));
    }
    cv.notify_one();
    return true;
}

void UDPPeer::sender_thread_func() {
    while (true) {
        std::unique_lock<std::mutex> lock(send_queue_mutex);
        cv.wait(lock, [this] { return !send_msg_queue.empty() || stop; });

        if (stop && send_msg_queue.empty()) break;
        auto msg = std::move(send_msg_queue.front());
        send_msg_queue.pop();
        lock.unlock();
        send_msg(msg);
    }
}

void UDPPeer::receiver_thread_func() {
    while (true) {
        if (stop) break;
        std::string msg;
        if (recv_msg(msg)) {
            {
                std::lock_guard<std::mutex> lock(recv_queue_mutex);
                recv_msg_queue.emplace(std::move(msg));
            }
            if (notify_receive_msg.has_value()) {
                notify_receive_msg.value()();
            }
        } else {
            log_error("Fail to receive message.");
        }
    }
}

Message UDPPeer::receive_msg() {
    std::lock_guard<std::mutex> lock(recv_queue_mutex);
    if (recv_msg_queue.empty()) {
        return Message::make_empty();
    }
    auto msg = std::move(recv_msg_queue.front());
    recv_msg_queue.pop();
    return Message::deserialize(msg);
}

void UDPPeer::close_connection() {
    // #TODO
    close_socket();
    return;
}

void UDPPeer::set_on_receive_msg(std::function<void()> on_receive_msg) {
    notify_receive_msg = on_receive_msg;
}

// close socket -> clear socket & port
void UDPPeer::close_socket() {
    if (socket_fd > 0) {
        close(socket_fd.value());
    }
    socket_fd.reset();
    local_port.reset();
}

bool UDPPeer::send_msg(const std::string &msg) {
    if (!socket_fd.has_value()) {
        log_error("This UDPPeer has NO socket fd!");
        return false;
    }
    if (!remote_addr.has_value() || !addr_len.has_value()) {
        log_error("This UDPPeer has NO remote address!");
        return false;
    }
    ssize_t send_ret = sendto(socket_fd.value(),
                              msg.c_str(),
                              msg.size(),
                              0,
                              &remote_addr.value(),
                              addr_len.value());
    if (send_ret < 0) {
        log_warning("Fail to send message.");
        return false;
    }
    return true;
}

bool UDPPeer::recv_msg(std::string &msg) {
    if (!socket_fd.has_value()) {
        log_error("This UDPPeer has NO socket fd!");
        return false;
    }

    // 设置接收缓冲区
    char buffer[65536];  // UDP最大包大小
    struct sockaddr recv_addr;
    socklen_t recv_len = sizeof(recv_addr);

    // 接收数据
    ssize_t recv_ret = recvfrom(
        socket_fd.value(), buffer, sizeof(buffer), 0, &recv_addr, &recv_len);

    if (recv_ret < 0) {
        log_warning("Fail to receive message.");
        return false;
    }

    // 将接收到的数据转换为string
    msg = std::string(buffer, recv_ret);

    // 可选:保存发送方地址信息(如果需要回复的话)
    if (!remote_addr.has_value()) {
        remote_addr = recv_addr;
        addr_len = recv_len;
    }

    return true;
}
