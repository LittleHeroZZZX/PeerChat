#include "udp_peer.h"
int main() {
    UDPPeer peer = UDPPeer(10333);
    peer.initialize();
    peer.set_remote("127.0.0.1", 22233);
    auto sender_info = SenderInfo();
    auto msg = Message::make_text(std::move(sender_info), "test");
    while (1) {
        peer.send_msg_async(msg);
    }
    return 0;
}