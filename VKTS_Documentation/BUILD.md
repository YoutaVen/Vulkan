How to build the VulKan ToolS (VKTS) examples:
----------------------------------------------

General requirements:
---------------------
  
- [LunarG Vulkan SDK](http://vulkan.lunarg.com) version has to be 1.0.8.0.
- Vulkan headers and libraries have to be visible by the used compiler and/or IDE. 


Following requirements do depend on the platform:

![Android](images/android_logo.png) Android:
---

- Building: ndk-build
    - Windows batch files included.
    - [Android NDK](http://developer.android.com/ndk/index.html) with Vulkan support required.
    - [Apache Ant](https://ant.apache.org/) required.
      

![Linux](images/linux_logo.png) Linux:
---

- Building: Eclipse IDE for C/C++ Developers (recommended)
    - [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/) project files included. 
- Building: make  
    - Minimum [CMake](https://cmake.org/) version has to be 3.2.0.
- Needed libraries: libx11-xcb-dev libxrandr-dev


![Windows](images/windows_logo.png) Windows:
---

- Building: Visual C++ (recommended)
    - Minimum [CMake](https://cmake.org/) version has to be 3.2.0.
    - [Visual Studio 2015](https://www.visualstudio.com/en-us/downloads/visual-studio-2015-downloads-vs.aspx) or later is required.
- Building: Eclipse IDE for C/C++ Developers
    - [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/) project files included. 
- Building: make  
    - Minimum CMake version has to be 3.2.0.
    - [MinGW-w64 - for 32 and 64 bit Windows](https://sourceforge.net/projects/mingw-w64/) required.


---

Using Eclipse:
--------------

1. Install [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/) for Linux or Windows.
2. Install GCC for Linux or [MinGW-w64 - for 32 and 64 bit Windows](https://sourceforge.net/projects/mingw-w64/) for Windows.
3. Import all folders as an existing project in Eclipse. 
4. Set the build configuration of VKTS and the VKTS_Example* in Eclipse to your operating system.
5. Build VKTS.
6. Build VKTS_Example*.
7. The executables are located in the [VKTS_Binaries](VKTS_Binaries/README.md) folder.


---

Using Visual C++ 2015:
----------------------

1. Install [Visual Studio 2015 Community Edition](https://www.visualstudio.com/en-us/downloads/visual-studio-2015-downloads-vs.aspx) for Windows.
2. Run CMake and configure for "Visual Studio 14 2015 Win64".
   Add `-DVKTS_WSI=VKTS_NO_VISUAL` to create without windowing system.
3. Set the source code directory to VKTS.
4. Set the build directory to VKTS/MSVC (Note: This build directory is expected by the examples).
5. Open the Visual C++ solution file and build VKTS.
6. Do step 3 to 5 with VKTS_Example*.
7. The executables are located in the [VKTS_Binaries](VKTS_Binaries/README.md) folder.


---

Using make:
-----------

1. Install a GNU GCC toolchain.
2. Run CMake and configure "MSYS Makefiles" for Windows or "Unix Makefiles" for Linux.
   Add `-DVKTS_WSI=VKTS_NO_VISUAL` to create without windowing system.
3. Set the source code directory to VKTS.
4. Set the build directory to VKTS (Note: This build directory is expected by the examples). 
5. Run make and build VKTS.
6. Do step 3 to 5 with VKTS_Example*.
7. The executables are located in the [VKTS_Binaries](VKTS_Binaries/README.md) folder.


---

Using Android NDK:
------------------

The needed shaders, textures and scene files are automatically copied into the assets folder by
executing `Build_Install.bat`. After the build, the assets are packed in the apk file. 

Note:
With the current beta NDK, copy the `include/vulkan` and `lib/libvulkan.so` from `android-24` to `android-23`,
as `android-23` is still used.

The following command builds the VKTS library:

1. Open console in `Android/jni` project folder and execute following command:
    - `ndk-build`


The following command creates the Android project:

1. Open console in `Android` project folder and execute following command:
    - `Init.bat`

The following command copies assets, builds and installs the Android project:

2. Open console in `Android` project folder and execute following command:
    - `Build_Install.bat`

The following commands allows to display the log output:

3. Open another console execute following command in the `VKTS_Binaries` folder:
    - `android_log.bat`

The following command executes the Android application:

4. Open console in `Android` project folder and execute following command:
    - `Execute.bat`


Setting up the Android device:
------------------------------

The following has only be done once:

1. Enable developer mode on the Android device.

The following has only be done once, if automatic TCPIP debugging is wanted:

1. Enable the wireless network on the Android device.
2. Execute the following commands without the comments:
    - adb root                         // Restart adb as root
    - adb remount                      // Remount /system for read and write
    - adb pull /system/build.prop      // Get the provided file
3. Append 'service.adb.tcp.port=5555' in donwloaded build.prop to autostart adbd in TCPIP mode
4. Execute the following command without the comment:
    - adb push build.prop /system      // Store the modified file


Common adb commands:
--------------------

- List Android devices:
    - adb devices
- Restart adbd daemon with usb:
    - adb usb
- Restart adbd daemon with tcpip and given port:
    - adb tcpip 5555
- Connect to a TCPIP enabled device:
    - adb connect 192.168.0.12:5555    // Change the IP address
