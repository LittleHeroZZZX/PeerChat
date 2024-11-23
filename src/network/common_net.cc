#include "common_net.h"
#include <arpa/inet.h>

void fill_address(std::optional<std::string> address,
                  in_port_t port,
                  sockaddr_in &sock_address) {
    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(port);
    if (address->empty()) {
        sock_address.sin_addr.s_addr = INADDR_ANY;
    } else {
        inet_pton(AF_INET, address->c_str(), &sock_address.sin_addr);
    }
}

void fill_address(std::optional<std::string> address,
                  std::string port,
                  sockaddr_in &sock_address) {
    in_port_t port_number = static_cast<in_port_t>(std::stoi(port));
    fill_address(address, port_number, sock_address);
}
