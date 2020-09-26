# AtomicChessOS

# ATC CONTROLLER

The controller is the main system on the embedded device that controls the hardware of a ATC Table

* Motor Control
* Communcication with the Server
* Communication with the UI running on the device
* Chess Figure Movement Algorithm

## REQUIREMENTS

* windows pc for development
* linux pc with buildroot toolchain
* embedded system for deployment

## SETUP A PROJECT USING VISUAL STUDIO AND VisualGDB

### REQUIREMENTS

On the development windows pc is the following software installed:

* Visual Studio 2019 (Community or Enterprise)
* VisualGDB Plugin (Version 5.5, Linux Edition)
* A SSH Client (MobaXterm Personal Edition)
* git (Gitkraken)



### LINUX BUILD SYSTEM

On the linux build system, the VisualGSB plugin builds the project.
This machine needs all requirements for building the buildroot toolchain.
After building the buildroot toolchain using the `./buildroot/build.sh` script and a SSH connection an be established, the further configuration work can be done on the windows development pc.

To setup the toolchain in VisualGDB, the following steps are needed.


* `$ sudo apt-get install socat`
* Setup SSH connection
* Set toolchain paths for make, gcc, g++,..


### EMBEDDED SYSTEM DEPLOYMENT

Afer building the sources into an executable, the final binaries will be copied to the embedded system.
The embedded system is flashed with tha latest image build by buildroot.

In order to setup the VisualGDB Project, its nessessary that the embedded device can be accessed over SSH.
