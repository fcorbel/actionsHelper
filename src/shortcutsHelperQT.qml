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
      onTextChanged: {
        if (text[0] !== "/") {
          // console.log("Make fuzzy search")
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
      id: highlight
      Rectangle {
        width: 180; height: 40
        color: "red"; radius: 5
      }
    }
    Component {
      id: delegate
      Item {
        width: list.width
        height: 60
        /*color: "green"*/
        Text {
          id: shortcutText
          text: shortcut
          font.weight: Font.Bold
          font.pixelSize: 20
        }
        Text {
          anchors.top: shortcutText.bottom
          anchors.left: parent.left
          anchors.leftMargin: 60
          width: parent.width - anchors.leftMargin
          text: content
          wrapMode: Text.WordWrap
        }
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
          model: shortcutsModel
          delegate: delegate
          highlight: highlight
          focus: true
          /*currentIndex: 1*/
      }
    }
  }
} 
