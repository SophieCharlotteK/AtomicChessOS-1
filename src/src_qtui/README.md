# AtomicChessOS


# BRANCH - QTUI

This branch contains the QT5 User Interface which is running on the RaspberryPi Display.



## TASK OF THE UI

* simple editable
* provide simple menu structure with max two menu layers
* player browser
* start game /stop game

### MENUS



### MOCKUPS

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
## WHY QT


### USING QtDesignStudio
* from mockup to qml stuff
* easy drag&drop icons
* unique naming of object
* 




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
 


