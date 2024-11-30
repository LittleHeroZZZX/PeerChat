import QtQuick 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width: ListView.view.width
    height: 60

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Avatar {
            source: avatar
            size: 40
        }

        Column {
            Layout.fillWidth: true

            Text {
                text: nickname
                font.bold: true
            }

            Text {
                text: ip + ":" + port
                font.pixelSize: 12
                color: "gray"
            }
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: "#e0e0e0"
    }
}
