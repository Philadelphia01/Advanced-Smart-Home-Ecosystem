===================================================
  Smart Home GUI — Qt Build Instructions
===================================================

REQUIREMENTS
------------
  - Qt 5.15+ or Qt 6.x (either works)
  - A C++17 compiler (GCC, Clang, or MSVC)
  - qmake (comes with Qt)

HOW TO INSTALL QT
-----------------
  Windows:  Download Qt installer from https://www.qt.io/download-open-source
            Install "Desktop" component (includes Qt Widgets + qmake)

  Ubuntu/Debian:
            sudo apt-get install qt5-default qtbase5-dev build-essential

  Fedora/RHEL:
            sudo dnf install qt5-qtbase-devel gcc-c++

  macOS:
            brew install qt
            export PATH="/opt/homebrew/opt/qt/bin:$PATH"

HOW TO BUILD
------------
  1. Open a terminal in this folder (cpp-smarthome/gui/)
  2. Run:
       qmake SmartHomeGUI.pro
       make          (Linux/macOS)
       nmake         (Windows with MSVC)
       mingw32-make  (Windows with MinGW)

  3. Run the application:
       ./SmartHomeGUI          (Linux/macOS)
       SmartHomeGUI.exe        (Windows)

WHAT THE GUI INCLUDES
---------------------
  - Tabbed interface (one tab per room)
  - Device cards for every device with:
      * ON/OFF toggle button with green/red indicator
      * Brightness slider (Lights)
      * Temperature spin box (Thermostats)
      * Volume slider (Speakers, TVs)
      * Channel selector + input combo (TVs)
      * PIN entry with Unlock/Lock buttons (Door Locks)
      * Alarm code entry with Arm/Disarm buttons (Alarm System)
  - Automation Rules tab: run or enable/disable any scene
  - System Log tab: live log of all device actions
  - Toolbar: All ON / All OFF / Save / Load state
  - Dark theme throughout

ALTERNATIVE: USING Qt CREATOR (Recommended for beginners)
----------------------------------------------------------
  1. Install Qt Creator from https://www.qt.io/download-open-source
  2. Open Qt Creator → Open Project → select SmartHomeGUI.pro
  3. Click the green Run button

FILE STRUCTURE
--------------
  gui/
    main_gui.cpp        Entry point (QApplication)
    MainWindow.h/.cpp   Main window with tabs, toolbar, log
    DeviceWidget.h/.cpp Individual device control card
    AutomationPanel.h/.cpp  Automation rules panel
    LogRedirect.h/.cpp  Redirects std::cout to Qt log view
    SmartHomeGUI.pro    Qt project file
    README_Qt.txt       This file

  ../ (parent folder — included automatically by .pro)
    SmartDevice.h       Base class
    Light.h             Light device
    Thermostat.h        Thermostat device
    Speaker.h           Speaker device
    SecurityCamera.h    Security camera device
    TV.h                TV device
    DoorLock.h          Door lock device
    AlarmSystem.h       Alarm system device
    Room.h              Room container
    SmartHome.h         Home container
    AutomationRule.h    Automation rule engine
    FileManager.h       Save/load state
===================================================
