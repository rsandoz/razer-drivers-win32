# OpenRazer-Win32

Control Razer devices on Windows without Synapse or Chroma SDK using the open source OpenRazer driver.  This project provides wrapper files that fill in for the Linux kernel headers and convert OpenRazer into a Windows DLL.

### Building

Open OpenRazer.sln in the latest version of Visual Studio.  I use VS2019 Community Edition.

Build the desired OpenRazer-Win32 version.  There are also two example apps provided.  64 bit debug version is default.

### Using

Previous versions of this library used the WinUSB driver.  It does not use this driver anymore, so please uninstall the WinUSB driver from any devices you may have installed it on before using this library.

This project contains a main.cpp file which tests the various Chroma API calls on all Chroma devices found.

This project is only a software library with a crude demonstration app.  If you are a user who wants to control your Razer devices without Synapse, check out my OpenRGB project which uses this library:

https://gitlab.com/CalcProgrammer1/OpenRGB
