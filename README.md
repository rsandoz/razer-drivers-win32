# OpenRazer-Win32

Control Razer devices on Windows without Synapse or Chroma SDK using the open source OpenRazer driver.  This project provides wrapper files that fill in for the Linux kernel headers and convert OpenRazer into a Windows DLL.

### Building

Open OpenRazer.sln in the latest version of Visual Studio.  I use VS2019 Community Edition.

Build the desired OpenRGB version.  There are also two example apps provided.  64 bit debug version is default.

### Using

You must install the WinUSB driver on the Razer device interfaces used for control message communication.  The Zadig tool makes it easy to install this driver for any USB device connected to your system.

Start by obtaining the Zadig USB driver tool.  Start zadig.exe.

https://zadig.akeo.ie/

Select Options->List all devices (This queries all of the USB devices connected to your system)

The dropdown should now contain devices starting with "Razer..."  Select one of you Razer devices and select the last interface in the group.
Example:
Razer DeathAdder Elite (Interface 0)
Razer DeathAdder Elite (Interface 1)
Razer DeathAdder Elite (Interface 2)-> Select this one for instance

Select "WinUSB (v6.1.7600.16385)" in the Driver field
then select "Install Driver" from the DropDown Button, then press the button "Replace Driver"

This will install the WinUSB driver on that interface.  Don't pick interface 0 or interface 1 as those are utilized by the OS's HID driver for normal keyboard and mouse operations.

This project contains a main.cpp file which tests the various Chroma API calls on all Chroma devices found.
