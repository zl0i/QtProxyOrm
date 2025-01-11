import QtQuick
import QtQuick.Controls

ApplicationWindow {
   width: 1280
   height: 720
   visible: true
   title: qsTr("ProxyOrm App")

   Row {
      x: 20
      y: 20
      width: parent.width - 40
      height: parent.height - 40
      spacing: 20
      Column {
         width: 250
         height: parent.height
         spacing: 20
         Label {
            text: "orderModel"
         }

         ListView {
            id: _orderView
            width: parent.width
            height: parent.height - 120 - y
            clip: true
            spacing: 8
            model: orderModel
            delegate: Item {
               width: _orderView.width
               height: 25
               Row {
                  spacing: 8
                  Label {
                     text: qsTr("id: %1").arg(idRole)
                  }
                  Label {
                     text: qsTr("by: %1").arg(createdByRole)
                  }
                  Label {
                     text: qsTr("number: %1 total: %2").arg(numberRole).arg(totalRole)
                  }
               }
               Rectangle {
                  y: parent.height
                  width: parent.width
                  height: 1
                  color: "#C4C4C4"
               }
            }
         }
         Row {
            spacing: 10
            Button {
               text: "append"
               onClicked: () => orderModel.append()
            }
            Button {
               text: "remove"
               onClicked: () => orderModel.remove()
            }
         }
         Row {
            spacing: 10
            Button {
               text: "dataChange"
               onClicked: () => orderModel.dataChange()
            }
            Button {
               text: "reset"
               onClicked: () => orderModel.reset()
            }
         }
         Row {
            spacing: 10
            Button {
               text: "clear"
               onClicked: () => orderModel.clear()
            }
            Button {
               text: "update"
               onClicked: () => orderModel.update()
            }
         }
      }

      Column {
         width: 230
         height: parent.height
         spacing: 20
         Label {
            text: "staffModel"
         }

         ListView {
            id: _staffView
            width: parent.width
            height: parent.height - 120 - y
            clip: true
            spacing: 8
            model: staffModel
            delegate: Item {
               width: _orderView.width
               height: 25
               Row {
                  spacing: 8
                  Label {
                     text: qsTr("id: %1").arg(idRole)
                  }
                  Label {
                     text: qsTr("name: %1").arg(nameRole)
                  }
                  Label {
                     text: qsTr("title: %1 rate: %2").arg(titleRole).arg(rateRole)
                  }
               }
               Rectangle {
                  y: parent.height
                  width: parent.width
                  height: 1
                  color: "#C4C4C4"
               }
            }
         }
         Row {
            spacing: 10
            Button {
               text: "append"
               onClicked: () => staffModel.append()
            }
            Button {
               text: "remove"
               onClicked: () => staffModel.remove()
            }
         }
         Row {
            spacing: 10
            Button {
               text: "dataChange"
               onClicked: () => staffModel.dataChange()
            }
            Button {
               text: "reset"
               onClicked: () => staffModel.reset()
            }
         }
         Row {
            spacing: 10
            Button {
               text: "clear"
               onClicked: () => staffModel.clear()
            }
            Button {
               text: "update"
               onClicked: () => staffModel.update()
            }
         }
      }

      Column {
         width: 230
         height: parent.height
         spacing: 20
         Label {
            text: "userModel"
         }

         ListView {
            id: _userView
            width: parent.width
            height: parent.height - 120 - y
            clip: true
            spacing: 8
            model: userModel
            delegate: Item {
               width: _userView.width
               height: 25
               Label {
                  text: qsTr("id: %1 %2 %3 email: %4").arg(idRole).arg(firstnameRole).arg(lastnameRole).arg(emailRole)
               }

               Rectangle {
                  y: parent.height
                  width: parent.width
                  height: 1
                  color: "#C4C4C4"
               }
            }
         }
         Row {
            spacing: 10
            Button {
               text: "append"
               onClicked: () => userModel.append()
            }
            Button {
               text: "remove"
               onClicked: () => userModel.remove()
            }
         }
         Row {
            spacing: 10
            Button {
               text: "dataChange"
               onClicked: () => userModel.dataChange()
            }
            Button {
               text: "reset"
               onClicked: () => userModel.reset()
            }
         }
         Row {
            spacing: 10
            Button {
               text: "clear"
               onClicked: () => userModel.clear()
            }
            Button {
               text: "update"
               onClicked: () => userModel.update()
            }
         }
      }

      Column {
         width: parent.width - x
         height: parent.height
         spacing: 20
         Label {
            text: "proxyValue :" + totalOrder.value
         }
         Label {
            text: "proxyModel"
         }

         ListView {
            id: _proxyView
            width: parent.width
            height: parent.height - y
            clip: true
            spacing: 8
            model: proxyModel
            delegate: Item {
               width: _proxyView.width
               height: 45

               Column {
                  width: parent.width
                  height: parent.height
                  spacing: 8

                  Label {
                     text: qsTr("id: %1 number: %2 total: %3 user: %4 %5").arg(idRole).arg(numberRole).arg(totalRole).arg(firstnameRole).arg(
                              lastnameRole)
                  }
                  Label {
                     text: qsTr("staff: %1 rate: %2 count: %3 total: %4 case: %5").arg(staffNameRole).arg(staffRateRole).arg(countOrderRole).arg(
                              userOrderTotalRole).arg(caseRole)
                  }
               }

               Rectangle {
                  y: parent.height
                  width: parent.width
                  height: 1
                  color: "#C4C4C4"
               }
            }
         }
      }
   }
}
