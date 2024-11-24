import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FluentUI
import PeerChat

FluWindow {
    id: root
    visible: true
    width: 800
    height: 600
    title: "网络主机程序"

    // 添加状态属性
    property bool isConnected: false
    property bool isValidIP: false
    property bool isValidPort: false

    // IP地址验证函数
    function validateIP(ip) {
        if (!ip)
            return false;
        var ipRegex = /^(\d{1,3}\.){3}\d{1,3}$/;
        if (!ipRegex.test(ip))
            return false;

        var parts = ip.split('.');
        return parts.every(function (part) {
            var n = parseInt(part, 10);
            return n >= 0 && n <= 255;
        });
    }

    // 端口号验证函数
    function validatePort(port) {
        if (!port)
            return false;
        var portNum = parseInt(port, 10);
        return portNum > 0 && portNum <= 65535;
    }

    RowLayout {
        id: row_layout
        anchors.fill: parent
        spacing: 10

        // 左侧连接部分
        Rectangle {
            Layout.preferredWidth: 170
            Layout.fillHeight: true
            Layout.margins: 10
            radius: 5

            ColumnLayout {
                id: connectLayout
                width: 150
                anchors.centerIn: parent
                spacing: 10

                FluCopyableText {
                    text: "本地开放端口：" + networkManager.local_port || "unknown"
                }

                FluTextBox {
                    id: ipInput
                    placeholderText: "输入对方IP"
                    enabled: root.isConnected ? false : true
                    Layout.fillWidth: true
                    onTextChanged: {
                        root.isValidIP = validateIP(text);
                        if (!root.isValidIP) {
                            helperText = "请输入有效的IP地址";
                        } else {
                            helperText = "";
                        }
                    }
                }

                FluTextBox {
                    id: portInput
                    placeholderText: "输入端口号"
                    enabled: root.isConnected ? false : true
                    Layout.fillWidth: true
                    onTextChanged: {
                        root.isValidPort = validatePort(text);
                        if (!root.isValidPort) {
                            helperText = "端口号范围：1-65535";
                        } else {
                            helperText = "";
                        }
                    }
                }

                FluComboBox {
                    id: protocolSelect
                    model: ["TCP", "UDP"]
                    Layout.fillWidth: true
                    enabled: root.isConnected ? false : true
                    onCurrentTextChanged: {}
                }

                FluLoadingButton {
                    id: connectButton
                    objectName: "connectButton"
                    text: root.isConnected ? "断开" : "连接"
                    loading: false
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignRight
                    enabled: root.isValidIP && root.isValidPort
                    onClicked: {
                        if (!root.isConnected) {
                            loading = true;
                            enabled = false;
                            connectSignal(ipInput.text, portInput.text, protocolSelect.currentText);
                        } else {
                            root.isConnected = false;
                            text = "连接";
                            disconnectSignal();
                        }
                    }
                    Connections {
                        target: networkManager
                        function onConnectedChanged(connected) {
                            root.isConnected = connected;
                            if (connected) {
                                showSuccess("连接成功");
                                connectButton.loading = false;
                                connectButton.enabled = true;
                            } else {
                                showSuccess("已断开");
                            }
                        }
                    }
                    signal disconnectSignal
                    signal connectSignal(string ip, string port, string protocol)

                    onConnectSignal: networkManager.connectToHost(ip, port, protocol)
                }
            }
        }

        // 右侧消息部分
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10
            spacing: 10

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                FluMultilineTextBox {
                    id: messageLog
                    width: parent.width
                    height: parent.height
                    readOnly: true
                    wrapMode: Text.Wrap
                    placeholderText: "消息记录将显示在这里..."

                    function append(message) {
                        text = text + "\n" + message;
                    }
                }
                Connections {
                    target: networkManager
                    function onMessageReceived() {
                        messageLog.append(formatChatMessage("对方", Date.now() / 1000, networkManager.receiveMessage()));
                    }
                }
            }

            RowLayout {
                id: sendLayout
                Layout.fillWidth: true
                spacing: 10

                ScrollView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 3 * messageInput.font.pixelSize + 20
                    clip: true

                    FluMultilineTextBox {
                        id: messageInput
                        width: parent.width
                        height: parent.height
                        placeholderText: "输入消息（Enter 换行，Ctrl+Enter 发送）"
                        wrapMode: Text.Wrap
                    }
                }

                FluFilledButton {
                    id: sendButton
                    objectName: "sendButton"
                    text: "发送"
                    Layout.alignment: Qt.AlignBottom
                    Layout.fillWidth: false
                    enabled: (root.isConnected) && root.isValidIP && root.isValidPort && messageInput.text.trim() !== ""
                    onClicked: {
                        messageLog.append(formatChatMessage("我", Date.now() / 1000, messageInput.text));
                        sendMessage(messageInput.text);
                        messageInput.clear();
                    }
                    signal sendMessage(string message)

                    onSendMessage: networkManager.sendMessage(message)
                }

                Shortcut {
                    // 绑定 Ctrl+Enter 到发送按钮
                    sequence: StandardKey.Enter | StandardKey.Ctrl
                    enabled: sendButton.enabled
                    onActivated: {
                        sendButton.clicked();
                    }
                }
            }
        }
    }
    function formatChatMessage(nickname, timestamp, message) {
        var date = new Date(timestamp * 1000);
        var timeString = date.toLocaleTimeString(Qt.locale(), "hh:mm:ss");

        // 使用特殊字符和格式化来提高可读性
        var formattedMessage = "┌─────────────────────────────────────\n";
        formattedMessage += "│ " + nickname + " @ " + timeString + "\n";
        formattedMessage += "├─────────────────────────────────────\n";

        // 将消息内容分行并添加边框
        var messageLines = message.split('\n');
        for (var i = 0; i < messageLines.length; i++) {
            formattedMessage += "│ " + messageLines[i] + "\n";
        }

        formattedMessage += "└─────────────────────────────────────\n\n";

        return formattedMessage;
    }
}
