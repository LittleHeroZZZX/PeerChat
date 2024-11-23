#pragma once
#include <netinet/in.h>
#include <optional>
#include <string>

void fill_address(std::optional<std::string> address,
                  in_port_t port,
                  sockaddr_in &sock_address);

void fill_address(std::optional<std::string> address,
                  std::string port,
                  sockaddr_in &sock_address);
