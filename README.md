# VidCapLib

VidCapLib is a branch of: OpenPnP Capture.  
	https://raw.githubusercontent.com/openpnp
	https://github.com/openpnp/VidCapLib

It is cross platform native video capture library for C/cpp with a focus on machine vision. Its goals are:

* Native camera access on Windows, Linux and Mac. Implemented with DirectShow, V4L2 and AVFoundation respectively.
* Named device enumeration.
* Strong, repeatable, unique IDs.
* Format control with support for at least YUV and MJPEG. Compressed formats such as MJPEG allow the use of multiple USB cameras on a hub or root controller.
* Auto and manual exposure control.
* Auto and manual focus control.
* Simple, common C wrapper for the listed APIs.


# Features
| Feature                  |  Windows   |    macOS     |     Linux      |
| ------------------------ |:----------:|:------------:|:--------------:|
| Platform API             | DirectShow | AVFoundation |      V4L2      |
| Named Device Enumeration |    Yes     |     Yes      |      Yes       |
| Capturing                |    Yes     |     Yes      |      Yes       |
| MJPEG formats            |    Yes     |  Yes (dmb1)  |      Yes       |
| YUV formats              |    Yes     |     Yes      | Yes, YUYV/YUV2 |
| Exposure control         |    Yes     |     Yes      |      Yes       |
| Focus control            |    Yes     |     Yes      |      Yes       |
| Zoom control             |    Yes     |     Yes      |      Yes       |
| Gain control             |    Yes     |     Yes      |      Yes       |
| White balance control    |    Yes     |     Yes      |      Yes       |
| Common C API             |    Yes     |     Yes      |      Yes       |
| Framerate control        |     No     |      No      |       No       |
| Re-Enumeration           |     No     |      No      |       No       |


# Getting Started
Packages and binaries are available in [releases](https://github.com/openpnp/VidCapLib/releases).

Examples:
  - [QtCaptureTest](./QtCaptureTest/) is a cross platform test program for OpenPnP implemented with Qt.
  - VidCapLib includes simple examples for [Mac](./mac/tests/), [Linux](./linux/tests/), and [Windows](./win/tests/).

See below for information about Building VidCapLib. 

Documentation for VidCapLib needs improvement. We would love your [help!](https://github.com/htartisan/VidCapLib/edit/master/README.md)


# Building VidCapLib
## Dependencies
* CMAKE 3.1 or better
* MAKE (osx, linux)
* Visual Studio 2013 + NMake or Ninja Build (windows)
* NASM for building libjpeg-turbo (linux)
* libgtk-3-dev (linux, test program)

## Build instructions (Windows)
Run the batch file 'bootstrap.bat' and choose the desired build system (VisualStudio/nmake or Ninja). Make sure the compiler (Visual Studio) is in the search path. 

Go to the build directory and run nmake or ninja to build the library and the test application.

## Build instructions (OSX)
Run 'bootstrap_osx.sh'. Run make.

## Build instructions (Linux)
Run 'bootstrap_linux.sh'. Run make.


# Supporting Other Platforms
* Implement all PlatformXXX classes, like in the win or linux directories.
* PlatformContext handles device and internal frame buffer format enumeration.
* PlatformStream is responsible for capturing and decoding the camera stream to a 8-bit per channel RGB frame buffer.
* Statically link external dependencies.


# Releases
Releases are built automatically for new tags on all supported platforms using [Github Actions]((https://github.com/htartisan/VidCapLib)/blob/master/.github/workflows/build.yml). See https://github.com/htartisan/VidCapLib/releases/latest to download the latest.

# Platform Notes

## MacOS

On MacOS as of 10.15 Camera permission is needed to open the camera. The library will automatically
execute the permission request, but an Info.plist is required to exist in the application bundle.
An example Info.plist is:

```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>NSCameraUsageDescription</key>
	<string>VidCapLib needs permission to access the camera to capture images.</string>
</dict>
</plist>
```

You can reset the camera permissions in MacOS for testing purposes by running ` tccutil reset Camera`.
