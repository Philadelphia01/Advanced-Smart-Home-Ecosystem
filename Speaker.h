#ifndef SPEAKER_H
#define SPEAKER_H

#include "SmartDevice.h"
#include <iostream>

class Speaker : public SmartDevice {
private:
    int volume;

public:
    Speaker(const std::string& name, int initialVolume = 30)
        : SmartDevice(name), volume(initialVolume) {}

    void turnOn() override {
        isOn = true;
        std::cout << "[Speaker: " << name << "] Turned ON at volume " << volume << ".\n";
    }

    void turnOff() override {
        isOn = false;
        std::cout << "[Speaker: " << name << "] Turned OFF.\n";
    }

    void setVolume(int vol) {
        if (vol < 0 || vol > 100) {
            std::cout << "[Speaker: " << name << "] Invalid volume. Must be 0-100.\n";
            return;
        }
        volume = vol;
        std::cout << "[Speaker: " << name << "] Volume set to " << volume << ".\n";
    }

    void getStatus() const override {
        std::cout << "  Speaker     : " << name
                  << " | State: " << (isOn ? "ON " : "OFF")
                  << " | Volume: " << volume << "\n";
    }
};

#endif
