# AtomicChessOS


# BRANCH - QTUI

This branch contains the QT5 User Interface which is running on the RaspberryPi Display.



## TASK OF THE UI

* simple editable
* provide simple menu structure with max two menu layers
* player browser
* start game /stop game

### MENUS

## WHY QT

### QUICKGUIDE QML

```qml
Text {
            id: hb_headline_text
            objectName: "hb_headline_text"
            x: 318
            y: 18
            width: 164
            height: 34
            text: qsTr("MAIN MENU")
            visible: true            
            font.pixelSize: 30
        }
```


```qml
Button {
            id: hb_settings_button
            x: 731
            y: 4
            width: 61
            height: 61

            text: qsTr("BUTTON NAME")
            
            Connections {
                target: hb_settings_button
                function onClicked(_mouse){
                    ss_container.visible = true
                    main_menu.lb_settings_btn()
                }
            }
        }
```



### MOCKUP

For the first mockups, the tool `Balsamique Mockup 3` was used to create a wireframe model of the menus.
Though the requirement that the userinterface should be simple to create and operate, only simple GUI element was used (like buttons, labels, icons).
So the userinterface can be adapted in every other UI framework too.
It also habe to be simple to use for the user and every menu should only present the most important elements.

The UI conzept offers the users to do the following things:

* Start a random game
* Start a game with selected user
* Overview of game progress and state
* Abort game
 
After deciding which elements are necessary to map this functionality, a simple mockup of the user interface was created.

![MOCKUP_MAIN_MENU](./documentation_images/MAINMENU.png)

For example, this is the mockup of the MainMenu. It allowes the player, to start a random game, get the players list and a spinning indicator was used to show the state of the game search funktion.

The gearbox icon, redirects the user to the settings menu. This menu offers basic options to interact with the table.
In a normal usage situation this settings menu is not important, but it is mostly used for maintenance.
The settings menu is accessable in every other menu and the back button, redirects the user back to the last menu showed before opening the settings menu.

![MOCKUP_MAIN_MENU](./documentation_images/SETTINGS.png)

Also a general information menu was added, which shows the table software version, its hardware id (used for communcation with the cloud backend).
For debug reasons the last internal errormessage can be shown here.

![MOCKUP_MAIN_MENU](./documentation_images/INFO.png)

This are only three example menus created for the user interface.
After creating the mockups, `Qt Design Studio` was used to convert the Mockup into an `QML` represenation

### Qt Design Studio

`Qt` offers several different tool to create a user interface. In this case the `Qt Design Studio`, was used in the first step of implementing the UI form the mockup. It has some advantages over the `Qt Creator`, reagarding the design workflow. It is for example possible to direclty import Adobe Photoshop files, this feature was used for the creation of the icons used in the interface. No manually export of the icon from Adobe Photoshop and reimporting into Qt was needed.
It also supports live preview of the design as an running application, no compilation needed for reviewing the UI.

The software uses a simple drag&drop system for elements, like buttons, labels and self designed components. The element can be placed on a empty screen to build the ui from the ground up. It also provides integration for the `QtQuickControl II` component presets, which includes some more advance ui element like the animated spinner or progress bars.

While building the UI in the software, the equivalent QML code is generated which corresponds in an `.qml` file.
The QML file can now be loaded into a `Qt Quick - Application` project inside of the `Qt Creator` software. 


## Qt Creator
In `Qt Creator` the whole programming of the backend took place. Connection the events of the elements with a C++ backend which connects over the the `Inter Process Communication` with the rest of the system.

It is also possible to edit the QML code and add new QML components inside of `Qt Creator`, which offers also a QML visual editor.
Here all object which are designed to be changed with C++ code, become a unique name. This happens inside of the QML code with the `objectName` attribute.

For example the content of the version label on the info menu should be changeable, so its gets a unique name with 
```qml
Text {
            id: is_hwid_label
            objectName: "is_hwid_label"
            x: 358
            y: 54
            color: "#ffffff"
            text: qsTr("HWID")
            font.pixelSize: 30
        }
```

With the `objectName` attribute set, it is possible modify other attributes inside of the element.
In the following C++ function is taken from the `menumanager.cpp` which is the C++ backend class between the QML and the `Inter Process Communication`.
The funktion changes the visible attribute of an element, which is searched with the `objectName` attribute.

```c++
void MenuManager::set_visible_element(QString _name, bool _state){
    QObject* obj = this->parent()->findChild<QObject*>(_name);
    if(obj){
        obj->setProperty("visible",_state);
    }else{
        qInfo()<< "cant get element" << _name;
    }
}
```

The `visible` attributes of an element, defines if the element is visible on the ui or not.
In the case of the UI for this project. Each menu is placed inside of an container.
If the user open a specific menu, all other menus will be hidden by setting the `visible` attribute to `false`, ecept of the target menu.

For this purpose each menu container has its own `objectName` attribute.

* `mm_container`, is the main menu container
* `ss_container`, is the settings menu container
* `ls_container`, is the start screen container

To finally switch the a menu the function `switch_menu(QString _container_name)` can be used, for simple menu switching.
It also poulates a variable called `last_menu_opened`, this stores the menu name the user comes from and makes its possible to switch back to the previous menu.
This functionally comes handy, for the settings menu, which can be opened from every other menu and by closing the settings menu the user redirect back to the prevous menu.

```c++
switch_menu("ls_container"); //SHOW LOGIN MENU
switch_menu("ss_container"); //SHOW SETTINGS MENU

switch_menu(last_menu_opened); //SWITCH BACK TO LOGIN MENU (= PREVIOUS MENU)
```







With the `Qt`










### PROJECT SETUP


#### TOOLCHAIN SETUP
#### DEVICE SETUP

### IMPORT QML COMPONENTS
* create qt qml prject
* import qml from design studio

### C++ BACKEBD
*interaction beween qml action guicommunication
* timer for polling

#### INTER PROCESS COMMUNICATION
* function gui manager


##### Protobuf
* message types
* needed information
* generation of the .proto file to cpp code
* location of the files in shared folder
##### Webserver
* advantage for debugging
* zeromq not wokring


### DEPLOYMENT
* using buildroot pacakge
 


