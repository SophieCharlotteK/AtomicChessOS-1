import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick3D 1.15
import QtQuick.Layouts 1.11
//IMPORT THE C++ FILE
//import MainMenu 1.0



Rectangle {
    id: window
    width: 800
    height: 480
    color: "#000074c4"



    Rectangle {
        id: mm_container
        x: 0
        y: 69
        width: 800
        height: 411
        color: "#197ab7"

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
                state: "off"
                text: qsTr("SEARCHING FOR GAME")
                Connections{
                    target: sfp_searchenable_button
                    function onToggled(_state){
                        print(sfp_searchenable_button.state)
                        if (sfp_searchenable_button.state == "on"){
                            sfp_searchenable_button.state = "off";
                            main_menu.search_for_players_toggled(false) //DONT KNOW WHY ITS INVERTED...
                            sfp_indicator.visible = false;
                        }else{
                            sfp_searchenable_button.state = "on";
                            main_menu.search_for_players_toggled(true)
                            sfp_indicator.visible = true;
                        }
                    }
                }
            }

            BusyIndicator {
                id: sfp_indicator
                x: 47
                y: 162
                width: 144
                height: 144
                wheelEnabled: true
                visible: false
                Connections{
                    target: sfp_indicator
                }
            }

            Text {
                id: spf_timer_text
                x: 77
                y: 25
                width: 107
                height: 50
                text: qsTr("0:00")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 30
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
                id: mm_search_player_btn
                x: 26
                y: 81
                width: 207
                height: 55
                text: qsTr("SEARCH PLAYERS")

                Connections {
                    target: mm_search_player_btn
                    function onClicked(_mouse){
                        main_menu.goto_search_player_view()
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
                        main_menu.start_ai()
                    }
                }
            }
        }

    }


}

/*##^##
Designer {
    D{i:0;3d-active-scene:-1}
}
##^##*/

