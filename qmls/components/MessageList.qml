import QtQuick 2.15
import QtQuick.Controls 2.15

ListView {
    id: messageListView
    // anchors.fill: parent
    clip: true
    model: ListModel {}
    delegate: MessageBubble {}
    spacing: 10

    // 添加一个方法来更新消息
    function updateMessages(messages) {
        model.clear();
        for (var i = 0; i < messages.length; i++) {
            model.append(messages[i]);
        }
    }

    // 让消息从底部开始显示
    verticalLayoutDirection: ListView.BottomToTop
    flickableDirection: Flickable.VerticalFlick
    onCountChanged: {
        positionViewAtEnd();
    }
}
