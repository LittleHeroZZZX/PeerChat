#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include "udp_peer.h"

#define LOCAL_PORT 22136

class NetworkManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected WRITE setConnected NOTIFY
                   connectedChanged)
    Q_PROPERTY(quint16 local_port READ getLocalPort WRITE setLocalPort NOTIFY
                   localPortChanged)
    QML_ELEMENT

  public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    bool isConnected() const;
    void setConnected(bool connected);

    quint16 getLocalPort() const;
    void setLocalPort(quint16 port);

    Q_INVOKABLE void disconnect();
    Q_INVOKABLE QString receiveMessage();

  public slots:
    Q_INVOKABLE void connectToHost(const QString &ip,
                                   const QString &port,
                                   const QString &protocol);
    Q_INVOKABLE void sendMessage(const QString &message);

  signals:
    void connectedChanged(const bool connected);
    void disconnected();
    void messageReceived();
    void errorOccurred(const QString &error);
    void localPortChanged(quint16 port);
    void sendSuccess();

  private:
    UDPPeer m_udpPeer;
    bool m_connected;
    quint16 m_localPort;

    void setupConnections();
};
