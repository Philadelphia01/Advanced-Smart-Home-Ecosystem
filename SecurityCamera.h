#ifndef SECURITYCAMERA_H
#define SECURITYCAMERA_H

#include "SmartDevice.h"
#include <iostream>

class SecurityCamera : public SmartDevice {
private:
    bool isRecording;

public:
    SecurityCamera(const std::string& name)
        : SmartDevice(name), isRecording(false) {}

    void turnOn() override {
        isOn = true;
        isRecording = true;
        std::cout << "[Camera: " << name << "] Turned ON and started recording.\n";
    }

    void turnOff() override {
        isOn = false;
        isRecording = false;
        std::cout << "[Camera: " << name << "] Turned OFF. Recording stopped.\n";
    }

    void getStatus() const override {
        std::cout << "  Camera      : " << name
                  << " | State: " << (isOn ? "ON " : "OFF")
                  << " | Recording: " << (isRecording ? "YES" : "NO") << "\n";
    }
};

#endif
