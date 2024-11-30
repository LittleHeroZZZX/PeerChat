import QtQuick 2.15
import QtQuick.Layouts 1.15
import "." as Components

Item {
    id: root
    width: ListView.view.width
    height: Math.max(avatarColumn.height, messageBubble.height) + 20

    Column {
        id: avatarColumn
        spacing: 5
        anchors {
            left: isOwnMessage ? undefined : parent.left
            right: isOwnMessage ? parent.right : undefined
            top: parent.top
            margins: 10
        }

        Components.Avatar {
            id: avatarImage
            source: avatar
            size: 40
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: nameText
            text: userName
            font.pixelSize: 12
            color: "#555555"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        id: messageBubble
        width: Math.min(messageText.implicitWidth + 40, root.width * 0.7)
        height: messageText.implicitHeight + timeText.height + 30
        color: isOwnMessage ? "#DCF8C6" : "#EFEFEF"
        radius: 10
        anchors {
            left: isOwnMessage ? undefined : avatarColumn.right
            right: isOwnMessage ? avatarColumn.left : undefined
            top: parent.top
            margins: 10
        }

        Text {
            id: messageText
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 10
            }
            text: message
            wrapMode: Text.Wrap
        }

        Text {
            id: timeText
            text: {
                var date = new Date(sentTime);
                return Qt.formatTime(date, "hh:mm");
            }
            font.pixelSize: 10
            color: "#888888"
            anchors {
                right: isOwnMessage ? undefined : parent.right
                left: isOwnMessage ? parent.left : undefined
                bottom: parent.bottom
                leftMargin: isOwnMessage ? 10 : 0
                rightMargin: isOwnMessage ? 0 : 10
                bottomMargin: 10
            }
        }
    }
}
