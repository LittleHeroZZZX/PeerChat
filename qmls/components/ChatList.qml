import QtQuick 2.15
import QtQuick.Controls 2.15

ListView {
    id: chatListView
    clip: true
    interactive: true
    highlightFollowsCurrentItem: true
    model: ListModel {
        ListElement {
            avatar: "file:///C:/Users/ZhouXin/projects/homework/networkProgram/peerchat/resources/avatars/user1.png"
            nickname: "用户1"
            ip: "192.168.1.100"
            port: "8080"
        }
        ListElement {
            avatar: "file:///C:/Users/ZhouXin/projects/homework/networkProgram/peerchat/resources/avatars/user2.png"
            nickname: "用户2"
            ip: "192.168.1.101"
            port: "8081"
        }
    }
    delegate: ChatListItem {
        MouseArea {
            anchors.fill: parent
            onClicked: {
                chatListView.currentIndex = index;
            }
        }
    }
    highlight: Rectangle {
        color: "lightblue"
    }
    focus: true

    // 添加一个信号，当选择改变时发出
    signal chatSelected(string nickname)

    onCurrentIndexChanged: {
        console.log("当前选择的索引:", currentIndex);
        if (currentIndex >= 0) {
            chatSelected(model.get(currentIndex).nickname);
        }
    }
}
