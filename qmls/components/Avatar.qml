import QtQuick 2.15
import QtGraphicalEffects 1.15

Item {
    id: root
    property string source
    property int size: 40

    width: size
    height: size

    Image {
        id: image
        anchors.fill: parent
        source: root.source
        fillMode: Image.PreserveAspectCrop
        visible: false
    }

    Rectangle {
        id: mask
        anchors.fill: parent
        radius: width / 2
        visible: false
    }

    OpacityMask {
        anchors.fill: image
        source: image
        maskSource: mask
    }
}
