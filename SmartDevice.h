#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include <string>

class SmartDevice {
protected:
    std::string name;
    bool isOn;

public:
    SmartDevice(const std::string& deviceName)
        : name(deviceName), isOn(false) {}

    virtual ~SmartDevice() {}

    virtual void turnOn() {
        isOn = true;
        std::cout << "[" << name << "] Turned ON.\n";
    }

    virtual void turnOff() {
        isOn = false;
        std::cout << "[" << name << "] Turned OFF.\n";
    }

    virtual void getStatus() const = 0;

    std::string getName() const { return name; }
    bool getIsOn() const { return isOn; }
};

#endif
