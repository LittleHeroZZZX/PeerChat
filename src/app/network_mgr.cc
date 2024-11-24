#include "network_mgr.h"
#include "peer_message.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    setupConnections();
}

NetworkManager::~NetworkManager() { m_udpPeer.close_connection(); }

bool NetworkManager::isConnected() const { return m_connected; }

void NetworkManager::setConnected(bool connected) {
    if (m_connected != connected) {
        m_connected = connected;
        emit connectedChanged(m_connected);
    }
}

quint16 NetworkManager::getLocalPort() const { return m_localPort; }

void NetworkManager::setLocalPort(quint16 port) {
    if (m_localPort != port) {
        m_localPort = port;
        emit localPortChanged(port);
    }
}

void NetworkManager::disconnect() {
    m_udpPeer.close_connection();
    emit disconnected();
}

void NetworkManager::connectToHost(const QString &ip,
                                   const QString &port,
                                   const QString &protocol) {
    if (m_udpPeer.set_remote(ip.toStdString(), port.toInt())) {
        if (!m_udpPeer.initialized()) {
            if (!m_udpPeer.initialize(m_localPort)) {
                emit errorOccurred("Failed to initialize connection");
                log_error("Failed to initialize connection");
            }
        }
        if (m_udpPeer.initialized()) {
            setConnected(true);
        }
    } else {
        emit errorOccurred("Invalid remote address");
        log_error("Invalid remote address");
    }
}

void NetworkManager::sendMessage(const QString &message) {
    if (!m_udpPeer.send_msg_async(
            Message::make_text(SenderInfo(), message.toStdString()))) {
        emit errorOccurred("Failed to send message");
    }
    emit sendSuccess();
}

QString NetworkManager::receiveMessage() {
    auto msg = m_udpPeer.receive_msg();
    return QString::fromStdString(msg.getContent());
}

void NetworkManager::setupConnections() {
    // Here you can set up any internal signal-slot connections
    // or callbacks from m_udpPeer to emit signals like messageReceived.
    m_udpPeer.set_on_receive_msg([this]() { emit messageReceived(); });
}
