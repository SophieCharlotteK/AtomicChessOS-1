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
