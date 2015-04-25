import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1



Item {
  MouseArea {
      anchors.fill: parent
      onClicked: {
          Qt.quit();
      }
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 4
    Layout.fillWidth: true

    TextField {
      objectName: "inputEl"
      placeholderText: "/help"
      Layout.fillWidth: true
      Layout.preferredHeight: 40
      focus: true
      Keys.onUpPressed: {
        if (list.currentIndex > 0) {
          list.currentIndex -= 1
        }
      }
      Keys.onDownPressed: {
        if (list.currentIndex < list.count - 1) {
          list.currentIndex += 1
        }
      }
      onTextChanged: {
        // No other simple way to implement application wide shortcut (without a menu/button)
        if ((text[length-1] === "q") && (event.modifiers === Qt.ControlModifier)) {
          console.log("Q")
        }
        if (text[0] !== "/") {
          qtCpp.makeSearch(text, "cosine", 0.1)
        }
      }
      Keys.onReturnPressed: {
        if (text[0] === "/") {
          qtCpp.processCmd(text)
        }
      }
    }
    Label {
      objectName: "titleEl"
      text: "hello"
      Layout.alignment: Qt.AlignCenter
    }

    Component {
      id: myDelegate
      Item {
        width: list.width
        height: 60
        Rectangle {
          id: backgroundS
          width: parent.width
          height: parent.height
          color: "#444"; radius: 5
        }
        Text {
          id: actionText
          text: action
          color: "steelblue"
          font.weight: Font.Bold
          font.pixelSize: 20
        }
        Text {
          anchors.top: actionText.bottom
          anchors.left: parent.left
          anchors.leftMargin: 60
          width: parent.width - anchors.leftMargin
          text: description
          color: "white"
          wrapMode: Text.WordWrap
        }
      }
    }
    Component {
      id: myHighlight
      Rectangle {
        height: 40
        color: "#ccc"; radius: 1
        opacity: 0.5
        z: myDelegate.z +1
      }
    }
    ScrollView {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.alignment: Qt.AlignBottom

      ListView {
          id: "list"
          spacing: 5
          anchors.fill: parent
          model: entriesModel
          delegate: myDelegate
          highlight: myHighlight
          focus: true
          /*currentIndex: 3*/
      }
    }
  }
} 
