import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: "#F0F0F0"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 10
        height: 50
        implicitHeight: 50  // 添加隐式高度

        TextField {
            id: messageInput
            Layout.fillWidth: true
            placeholderText: "输入消息..."
            onAccepted: sendButton.clicked()
        }

        Button {
            id: sendButton
            text: "发送"
            onClicked: {
                if (messageInput.text.trim() !== "") {
                    // 这里应该添加发送消息的逻辑
                    console.log("发送消息:", messageInput.text);
                    messageInput.text = "";
                }
            }
        }
    }
}
