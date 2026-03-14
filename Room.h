#ifndef ROOM_H
#define ROOM_H

#include "SmartDevice.h"
#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Room {
private:
    std::string roomName;
    std::vector<SmartDevice*> devices;

public:
    Room(const std::string& name) : roomName(name) {}

    void addDevice(SmartDevice* device) {
        devices.push_back(device);
        std::cout << "Device '" << device->getName() << "' added to room '" << roomName << "'.\n";
    }

    void turnAllOn() {
        std::cout << "\n--- Turning ALL devices ON in [" << roomName << "] ---\n";
        for (SmartDevice* d : devices) {
            d->turnOn();
        }
    }

    void turnAllOff() {
        std::cout << "\n--- Turning ALL devices OFF in [" << roomName << "] ---\n";
        for (SmartDevice* d : devices) {
            d->turnOff();
        }
    }

    void showAllStatus() const {
        std::cout << "\n  === Room: " << roomName << " ===\n";
        if (devices.empty()) {
            std::cout << "  (No devices in this room)\n";
        } else {
            for (const SmartDevice* d : devices) {
                d->getStatus();
            }
        }
    }

    std::string getName() const { return roomName; }

    const std::vector<SmartDevice*>& getDevices() const { return devices; }

    int getDeviceCount() const { return static_cast<int>(devices.size()); }
};

#endif
