import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11
//IMPORT THE C++ BACKEND
import MenuManager 1.0



Rectangle {
    id: window
    width: 800
    height: 480
    color: "#0074c4"
    z: -1

    MenuManager{
        id:main_menu
    }


    Rectangle {
        id: hb_container
        x: 0
        y: 0
        width: 800
        height: 69
        color: "#dbdbdb"

        Text {
            id: hb_headline_text
            x: 318
            y: 18
            width: 164
            height: 34
            text: qsTr("MAIN MENU")
            lineHeightMode: Text.ProportionalHeight
            fontSizeMode: Text.FixedSize
            font.pixelSize: 30
        }

        Text {
            id: hb_atc_text
            x: 83
            y: 18
            width: 164
            height: 34
            text: qsTr("ATC")
            font.pixelSize: 30
            fontSizeMode: Text.FixedSize
            lineHeightMode: Text.ProportionalHeight
        }

        Image {
            id: hb_atc_icon
            x: 8
            y: 4
            width: 61
            height: 61
            fillMode: Image.PreserveAspectFit
            source: "noun_Chess_1376847.png"

        }

        Button {
            id: hb_settings_button
            x: 731
            y: 4
            width: 61
            height: 61

            Image {
                id: hb_settings_icon
                x: 0
                y: 0
                width: 61
                height: 61
                fillMode: Image.PreserveAspectFit
                source: "noun_Settings_56313.png"
            }
        }

        Image {
            id: hb_connection_icon
            x: 671
            y: 4
            width: 61
            height: 61
            fillMode: Image.PreserveAspectFit
            source: "noun_Cloud_offline.png"


        }

    }

    Rectangle {
        id: mm_container
        x: 0
        y: 69
        width: 800
        height: 411
        color: "#197ab7"
        visible: false
        Rectangle {
            id: mm_searching_for_players_box
            x: 478
            y: 31
            width: 260
            height: 343
            color: "#48b3ff"
            Switch {
                id: sfp_searchenable_button
                x: 14
                y: 91
                text: qsTr("SEARCHING FOR GAME")
                Connections {
                    target: sfp_searchenable_button
                    function onToggled(_state){
                        print(sfp_searchenable_button.state)
                        if (sfp_searchenable_button.state == "on"){
                            sfp_searchenable_button.state = "off";
                            main_menu.mm_search_for_players_toggled(false) //DONT KNOW WHY ITS INVERTED...
                            sfp_indicator.visible = false;
                        }else{
                            sfp_searchenable_button.state = "on";
                            main_menu.mm_search_for_players_toggled(true)
                            sfp_indicator.visible = true;
                        }
                    }
                }
                state: "off"
            }

            BusyIndicator {
                id: sfp_indicator
                x: 47
                y: 162
                width: 144
                height: 144
                visible: false
                Connections {
                    target: sfp_indicator
                }
                wheelEnabled: true
            }

            Text {
                id: spf_timer_text
                x: 77
                y: 25
                width: 107
                height: 50
                text: qsTr("0:00")
                font.pixelSize: 30
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Rectangle {
            id: mm_sart_game_box
            x: 61
            y: 31
            width: 253
            height: 343
            color: "#48b3ff"
            Button {
                id: mm_show_playerlist_btn
                x: 26
                y: 81
                width: 207
                height: 55
                text: qsTr("SHOW AVARIABLE PLAYERS")
                Connections {
                    target: mm_show_playerlist_btn
                    function onClicked(_mouse){
                        main_menu.mm_player_list_btn()
                    }
                }
            }

            Button {
                id: mm_start_random_btn
                x: 26
                y: 183
                width: 207
                height: 55
                text: qsTr("START AI MATCH")
                Connections {
                    target: mm_start_random_btn
                    function onClicked(_mouse){
                        main_menu.mm_start_ai_btn()
                    }
                }
            }
        }
    }

    Rectangle {
        id: ls_container
        x: 0
        y: 69
        width: 800
        height: 411
        color: "#197ab7"
        visible: true
        DelayButton {
            id: ls_login_btn
            x: 272
            y: 147
            width: 257
            height: 118
            text: qsTr("BEGIN")
            font.pointSize: 20
            Connections {
                target: ls_login_btn
                function onClicked(_mouse){
                    main_menu.ls_login_btn()
                }
            }
        }
    }

    Rectangle {
        id: ss_container
        x: 0
        y: 68
        width: 800
        height: 411
        color: "#197ab7"
        visible: false
        Grid {
            id: ss_manual_function_buttons_grid
            x: 0
            y: 0
            width: 230
            height: 411
            layoutDirection: Qt.LeftToRight
            verticalItemAlignment: Grid.AlignVCenter
            horizontalItemAlignment: Grid.AlignHCenter
            padding: 17
            spacing: 68
            rows: 5
            columns: 1

            Button {
                id: ss_scan_board_btn
                width: 200
                height: 80
                text: qsTr("SCAN BOARD")
            }

            Button {
                id: ss_init_board_btn
                width: 200
                height: 80
                text: qsTr("INIT BOARD")
            }

            Button {
                id: ss_log_out_btn
                width: 200
                height: 80
                text: qsTr("LOGOUT")
            }
        }

        Grid {
            id: ss_manual_movement_btn_grid
            x: 362
            y: 8
            width: 430
            height: 429
            spacing: 15
            padding: 2
            horizontalItemAlignment: Grid.AlignHCenter
            verticalItemAlignment: Grid.AlignVCenter
            rows: 8
            columns: 8

            Button {
                id: button
                width: 40
                height: 40
                text: qsTr("A1")
            }

            Button {
                id: button1
                width: 40
                height: 40
                text: qsTr("B1")
            }

            Button {
                id: button2
                width: 40
                height: 40
                text: qsTr("C1")
            }

            Button {
                id: button3
                width: 40
                height: 40
                text: qsTr("D1")
            }

            Button {
                id: button4
                width: 40
                height: 40
                text: qsTr("E1")
            }

            Button {
                id: button5
                width: 40
                height: 40
                text: qsTr("F1")
            }

            Button {
                id: button6
                width: 40
                height: 40
                text: qsTr("G1")
            }

            Button {
                id: button7
                width: 40
                height: 40
                text: qsTr("H1")
            }

            Button {
                id: button8
                width: 40
                height: 40
                text: qsTr("A2")
            }

            Button {
                id: button9
                width: 40
                height: 40
                text: qsTr("B2")
            }
        }
    }



}

/*##^##
Designer {
    D{i:0;active3dScene:"-1"}D{i:9;invisible:true}D{i:24;invisible:true}
}
##^##*/

