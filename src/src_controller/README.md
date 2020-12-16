# AtomicChessOS

# ATC CONTROLLER

The controller is the main system on the embedded device that controls the hardware of a ATC Table

* Motor Control
* Communcication with the Server
* Communication with the UI running on the device
* Chess Figure Movement Algorithm


For this software component, we are using a Windows and Visual Studi environment for developing the software.
Our embedded system is a linux device, so we have to build the application for the linux environment running on the emmbedded device.
For this task, the prevously build buildroot toolchain is used.
A software plugin called VisualGDB for Visual Studio, enable Visual Studio to use the linux toolchain over a remote linux system, in order to build and deploy the application.

## REQUIREMENTS

* windows pc for development
* linux pc with buildroot toolchain
* embedded system for deployment

## SETUP A PROJECT USING VISUAL STUDIO AND VisualGDB


### REQUIREMENTS

On the development windows pc is the following software installed:
* Windows 10 Pro x64
* Visual Studio 2019 (Community or Enterprise)
* VisualGDB Plugin (Version 5.5, Linux Edition)
* A SSH Client (MobaXterm Personal Edition)
* git (Gitkraken)



### LINUX BUILD SYSTEM

On the linux build system, the VisualGSB plugin builds the project.
The system is a `Intel Nuc 10`, mini pc, with a stock `Ubuntu 20.04` installed.
One the machine are all package requirements for building the buildroot toolchain installed (see buildroot chapter).


After cloning the repository `$ git clone -b buildroot git@github.com:RBEGamer/AtomicChessOS.git ./atc_buildroot` and switched to the `buildroot` branch, the build process of the toolchain was invoked using the build script `$ ./atc_buildroot/src/src_buildroot/buildroot/build.sh`.

After building the  toolchain is completed, the further configuration work can be done on the windows development pc.
Its nessessary for the linux build system to allow SSH connections.
Its may required to install the OpenSSHServer uisng `$ sudo apt install openssh-server`.

The VisualGDB plugin requires some extra packages on the build pc:

* `$ sudo apt-get install socat`

Now the configuration of the linux build pc for the VisualGDB plugin is complete.
The next step, is the setup of the embedded system for deployment.

### EMBEDDED SYSTEM DEPLOYMENT

Afer building the sources into an executable, the final binaries will be copied to the embedded system.
The embedded system is flashed with tha latest image build by buildroot.

In order to setup the VisualGDB Project, its nessessary that the embedded device can be accessed over SSH.




## THIRD PARTY LIBRARIES

The ATC_Controller software uses serveral different third party libraries, escacially for http requests and json parsing.
* `https://github.com/nlohmann/json#serialization--deserialization`, json parsing.
* `https://github.com/yhirose/cpp-httplib`, http/https requests.
* `https://github.com/Neargye/magic_enum`, enum handling.
* `https://github.com/emilk/loguru`
A copy of the used versions, are located in the controller software directory.



# CONTROLLER OVERVIEW

## MOTOR DRIVER
## BOARD CLASS

## CHESS PIECE
### NFC TAG CONTENT GENERATOR
Each figures NFC tag is ued to identify the figure. For easy usage of the NFC tag, it is formatted into the NDEF format, so its simply possible to program the NFC tags with an mobile phone. The tag contains a ndef `text record` with a string to identifiy the type of figure (three bits), color (one bit) and a counting number (four bits). A combination of all three attributes make the figures id unique to the system. For the automatic unique ID generation, a html page `ChessFigureIDGenerator.html` was created to automaticliy generate all ID for a complete chess board.
Only one byte is stored in the record of the text record and so only 10bytes are used on the NFC tag.

* MSB [7,6,5,4,3,2,1,0] LSB 
* MSB [TYPE,TYPE,TYPE,COLOR,ID,ID,ID,ID] LSB 

For reading the NFC tag with the PN532, the program only reads the first NDEF record and parses the text record data to get the figure type. With this system it is possible to expand the system further and store more data on the NFC tag, by adding more NDEF records.

On the controller side two functions were created to generate the unique ids and read them back.

```c++
  ChessPiece::FIGURE fig;
	fig.figure_number = 1;
	fig.color = ChessPiece::COLOR_BLACK;
	fig.type = ChessPiece::TYPE_KNIGHT;
  
  //GENERATE ID FROM STRUCT
  uint8_t UNIQUE_ID = ChessPiece::figure2NDEF(fig);
  
  //PARSE UNIQUE_ID BACK TO A STRUCT
  ChessPiece::FIGURE back = ChessPiece::NDEF2Figure(UNIQUE_ID);
  
````
* IMAGE

## CONFIG AND CALIBRATION

The system uses a config file located in the filesystem `/usr/ATC/atccontroller.ini`.



### CALIBRATION

In order to calibrate the mechanic, several entries in the config file has to be changed.

![MOCKUP_MAIN_MENU](./documentation_images/ATC_Calibration_Guide.png)

### Chess Piece Movement Algorithm

![MOCKUP_MAIN_MENU](./documentation_images/ATC_ChessMoveAlgorithm.png)

#### BASIC MOVEMENT PROCEDURE

##### BOARD <-> BOARD

##### PARK_POSITION <-> BOARD || BOARD <-> PARK_POSITON

#### PIECE MOVE TIME

| ALGORITHM_V1_TRAVEL_TIME [s] 	| ALGORITHM_V2_TRAVEL_TIME [s] 	| TRAVEL_DISTANCE [FIELDS_DIAGONAL] 	|
|------------------------------	|------------------------------	|-----------------------------------	|
| 7.20                         	| 2.56                         	| 1                                 	|
| 11.56                        	| 6,20                         	| 3                                 	|
| 12,27                        	| 7,06                         	| 5                                 	|
| 14,39                        	| 6,56                         	| 8                                 	|


