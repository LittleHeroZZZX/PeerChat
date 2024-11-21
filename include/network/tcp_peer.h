#include "network/network_peer.h"
#include <string>

class TCPPeer : public NetworkPeer {
public:
  TCPPeer();
  ~TCPPeer();

  bool initialize(const std::string &address, int port) override;
  bool sendData(const std::string &data) override;
  std::string receiveData() override;
  void closeConnection() override;
  bool isConnected() const override;

private:
  // 维护 TCP 连接所需的成员变量
  int socket_fd;
  bool connected;
};
