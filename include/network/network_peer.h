#include <string>

class NetworkPeer {
public:
  virtual ~NetworkPeer() {}

  // 初始化网络
  virtual bool initialize(const std::string &address, int port) = 0;

  // 发送数据
  virtual bool sendData(const std::string &data) = 0;

  // 接收数据
  virtual std::string receiveData() = 0;

  // 关闭连接
  virtual void closeConnection() = 0;

  // 检查是否已连接
  virtual bool isConnected() const = 0;
};
