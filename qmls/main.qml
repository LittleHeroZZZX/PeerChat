import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "./components" as Components

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "聊天应用"

    // 创建一个 JavaScript 对象来存储所有聊天记录
    property var allChatMessages: {
        "用户1": [
            {
                avatar: "file:///C:/Users/ZhouXin/projects/homework/networkProgram/peerchat/resources/avatars/user1.png",
                message: "你好！",
                sentTime: new Date(),
                isOwnMessage: false,
                userName: "用户1"
            },
            {
                avatar: "file:///C:/Users/ZhouXin/projects/homework/networkProgram/peerchat/resources/avatars/user2.png",
                message: "你好！最近怎么样？",
                sentTime: new Date(),
                isOwnMessage: true,
                userName: "用户2"
            }
        ],
        "用户2": [
            {
                avatar: "file:///C:/Users/ZhouXin/projects/homework/networkProgram/peerchat/resources/avatars/user2.png",
                message: "嗨，在吗？",
                sentTime: new Date(),
                isOwnMessage: false,
                userName: "用户2"
            },
            {
                avatar: "file:///C:/Users/ZhouXin/projects/homework/networkProgram/peerchat/resources/avatars/user1.png",
                message: "在的，有什么事吗？",
                sentTime: new Date(),
                isOwnMessage: true,
                userName: "用户1"
            }
        ]
    }

    SplitView {
        anchors.fill: parent
        spacing: 0

        Components.ChatList {
            id: chatList
            SplitView.preferredWidth: 250
            SplitView.minimumWidth: 200
            SplitView.fillHeight: true
            onChatSelected: function (nickname) {
                messageList.updateMessages(allChatMessages[nickname]);
            }
        }

        ColumnLayout {
            SplitView.fillWidth: true

            Components.MessageList {
                id: messageList
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Components.InputArea {
                id: inputArea
                Layout.fillWidth: true
                Layout.preferredHeight: 50
            }
        }
    }
}
