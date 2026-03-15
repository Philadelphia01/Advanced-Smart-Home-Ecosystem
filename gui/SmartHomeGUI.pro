QT += core gui widgets

CONFIG += c++17
TARGET  = SmartHomeGUI
TEMPLATE = app

INCLUDEPATH += ..

SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    DeviceWidget.cpp \
    AutomationPanel.cpp \
    LogRedirect.cpp

HEADERS += \
    MainWindow.h \
    DeviceWidget.h \
    AutomationPanel.h \
    LogRedirect.h \
    ../SmartDevice.h \
    ../Light.h \
    ../Thermostat.h \
    ../Speaker.h \
    ../SecurityCamera.h \
    ../TV.h \
    ../DoorLock.h \
    ../AlarmSystem.h \
    ../Room.h \
    ../SmartHome.h \
    ../AutomationRule.h \
    ../FileManager.h
