import QtQuick 2.15
import QtQuick.Controls 2.15
import RectangleItem 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 800
    title: "OpenGL Rectangle Window"
    color: "green"

    Item {
        id: name
        // anchors.centerIn: parent
        // anchors.fill: parent
        width: 500
        height: 500
        RectangleItem {
            id: rectangleWindow
            anchors.fill: parent
            // width: 200
            // height: 150
            objectPosition: Qt.vector3d(0, 0, 0)
            rotate: 0
            scale: 1
            x: 50
            y: 50
            useTexture: false
        }
    }

    Button {
        text: rectangleWindow.useTexture ? "Disable Texture" : "Enable Texture"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10

        onClicked: {
            name.x += 50
            name.y += 50
            rectangleWindow.width += 100
            rectangleWindow.height += 100
        }
        // onClicked: rectangleWindow.setWindowSize(
        //                ) //rectangleWindow.useTexture = !rectangleWindow.useTexture
    }
}
