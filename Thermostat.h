#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "SmartDevice.h"
#include <iostream>

class Thermostat : public SmartDevice {
private:
    double temperature;

public:
    Thermostat(const std::string& name, double initialTemp = 22.0)
        : SmartDevice(name), temperature(initialTemp) {}

    void turnOn() override {
        isOn = true;
        std::cout << "[Thermostat: " << name << "] Turned ON. Current target: " << temperature << " C.\n";
    }

    void turnOff() override {
        isOn = false;
        std::cout << "[Thermostat: " << name << "] Turned OFF.\n";
    }

    void setTemperature(double temp) {
        if (temp < 10.0 || temp > 35.0) {
            std::cout << "[Thermostat: " << name << "] Invalid temperature. Must be 10-35 C.\n";
            return;
        }
        temperature = temp;
        std::cout << "[Thermostat: " << name << "] Temperature set to " << temperature << " C.\n";
    }

    void getStatus() const override {
        std::cout << "  Thermostat  : " << name
                  << " | State: " << (isOn ? "ON " : "OFF")
                  << " | Target Temp: " << temperature << " C\n";
    }
};

#endif
