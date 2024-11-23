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
        std::lock_guard<std::mutex> lock(queue_mutex);
        msg_queue.emplace(std::move(msg.serialize()));
    }
    cv.notify_one();
    return true;
}

void UDPPeer::sender_thread_func() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cv.wait(lock, [this] { return !msg_queue.empty() || stop; });

        if (stop && msg_queue.empty()) break;
        auto msg = std::move(msg_queue.front());
        msg_queue.pop();
        lock.unlock();
        send_msg(msg);
    }
}

std::string UDPPeer::receive_msg() { return ""; }

void UDPPeer::close_connection() {
    // #TODO
    return;
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
