#ifndef LIGHT_H
#define LIGHT_H

#include "SmartDevice.h"
#include <iostream>

class Light : public SmartDevice {
private:
    int brightness;

public:
    Light(const std::string& name, int initialBrightness = 50)
        : SmartDevice(name), brightness(initialBrightness) {}

    void turnOn() override {
        isOn = true;
        std::cout << "[Light: " << name << "] Turned ON at brightness " << brightness << "%.\n";
    }

    void turnOff() override {
        isOn = false;
        std::cout << "[Light: " << name << "] Turned OFF.\n";
    }

    void setBrightness(int level) {
        if (level < 0 || level > 100) {
            std::cout << "[Light: " << name << "] Invalid brightness. Must be 0-100.\n";
            return;
        }
        brightness = level;
        std::cout << "[Light: " << name << "] Brightness set to " << brightness << "%.\n";
    }

    void getStatus() const override {
        std::cout << "  Light       : " << name
                  << " | State: " << (isOn ? "ON " : "OFF")
                  << " | Brightness: " << brightness << "%\n";
    }
};

#endif
