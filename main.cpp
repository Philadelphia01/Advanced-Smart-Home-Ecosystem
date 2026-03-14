#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "SmartDevice.h"
#include "Light.h"
#include "Thermostat.h"
#include "Speaker.h"
#include "SecurityCamera.h"
#include "Room.h"
#include "SmartHome.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pauseAndContinue() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void deviceMenu(SmartDevice* device) {
    while (true) {
        std::cout << "\n--- Device: " << device->getName() << " ---\n";
        std::cout << "1. Turn ON\n";
        std::cout << "2. Turn OFF\n";
        std::cout << "3. View Status\n";

        Light* light = dynamic_cast<Light*>(device);
        Thermostat* thermo = dynamic_cast<Thermostat*>(device);
        Speaker* speaker = dynamic_cast<Speaker*>(device);

        if (light)   std::cout << "4. Set Brightness\n";
        if (thermo)  std::cout << "4. Set Temperature\n";
        if (speaker) std::cout << "4. Set Volume\n";

        std::cout << "0. Back\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 1:
                device->turnOn();
                break;
            case 2:
                device->turnOff();
                break;
            case 3:
                device->getStatus();
                break;
            case 4:
                if (light) {
                    std::cout << "Enter brightness (0-100): ";
                    int b; std::cin >> b; clearInput();
                    light->setBrightness(b);
                } else if (thermo) {
                    std::cout << "Enter temperature (10.0-35.0 C): ";
                    double t; std::cin >> t; clearInput();
                    thermo->setTemperature(t);
                } else if (speaker) {
                    std::cout << "Enter volume (0-100): ";
                    int v; std::cin >> v; clearInput();
                    speaker->setVolume(v);
                } else {
                    std::cout << "This device does not support adjustment.\n";
                }
                break;
            case 0:
                return;
            default:
                std::cout << "Invalid choice.\n";
        }
        pauseAndContinue();
    }
}

void roomMenu(Room* room) {
    while (true) {
        std::cout << "\n=== Room: " << room->getName() << " ===\n";
        std::cout << "1. View all device statuses\n";
        std::cout << "2. Turn ALL devices ON\n";
        std::cout << "3. Turn ALL devices OFF\n";
        std::cout << "4. Control a specific device\n";
        std::cout << "0. Back\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 1:
                room->showAllStatus();
                pauseAndContinue();
                break;
            case 2:
                room->turnAllOn();
                pauseAndContinue();
                break;
            case 3:
                room->turnAllOff();
                pauseAndContinue();
                break;
            case 4: {
                const auto& devices = room->getDevices();
                if (devices.empty()) {
                    std::cout << "No devices in this room.\n";
                    pauseAndContinue();
                    break;
                }
                std::cout << "\nDevices in " << room->getName() << ":\n";
                for (int i = 0; i < (int)devices.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << devices[i]->getName() << "\n";
                }
                std::cout << "  0. Back\n";
                std::cout << "Select device: ";
                int devChoice;
                std::cin >> devChoice;
                clearInput();
                if (devChoice >= 1 && devChoice <= (int)devices.size()) {
                    deviceMenu(devices[devChoice - 1]);
                } else if (devChoice != 0) {
                    std::cout << "Invalid selection.\n";
                    pauseAndContinue();
                }
                break;
            }
            case 0:
                return;
            default:
                std::cout << "Invalid choice.\n";
                pauseAndContinue();
        }
    }
}

void mainMenu(SmartHome& home) {
    while (true) {
        std::cout << "\n+========================================+\n";
        std::cout << "|   SMART HOME CONTROL: " << home.getName() << "\n";
        std::cout << "+========================================+\n";
        std::cout << "1. View entire home status\n";
        std::cout << "2. Turn ALL devices ON (whole home)\n";
        std::cout << "3. Turn ALL devices OFF (whole home)\n";
        std::cout << "4. Manage a specific room\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 1:
                home.showAllStatus();
                pauseAndContinue();
                break;
            case 2:
                home.turnAllOn();
                pauseAndContinue();
                break;
            case 3:
                home.turnAllOff();
                pauseAndContinue();
                break;
            case 4: {
                const auto& rooms = home.getRooms();
                if (rooms.empty()) {
                    std::cout << "No rooms in this home.\n";
                    pauseAndContinue();
                    break;
                }
                std::cout << "\nRooms:\n";
                for (int i = 0; i < (int)rooms.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << rooms[i]->getName()
                              << " (" << rooms[i]->getDeviceCount() << " devices)\n";
                }
                std::cout << "  0. Back\n";
                std::cout << "Select room: ";
                int roomChoice;
                std::cin >> roomChoice;
                clearInput();
                if (roomChoice >= 1 && roomChoice <= (int)rooms.size()) {
                    roomMenu(rooms[roomChoice - 1]);
                } else if (roomChoice != 0) {
                    std::cout << "Invalid selection.\n";
                    pauseAndContinue();
                }
                break;
            }
            case 0:
                std::cout << "Goodbye! Exiting Smart Home System.\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                pauseAndContinue();
        }
    }
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "  Advanced Smart Home Ecosystem v1.0\n";
    std::cout << "=========================================\n";
    std::cout << "Initializing system...\n\n";

    SmartHome myHome("My Smart Home");

    Room livingRoom("Living Room");
    Room bedroom("Bedroom");
    Room kitchen("Kitchen");
    Room garage("Garage");

    Light ceilingLight("Ceiling Light", 80);
    Light floorLamp("Floor Lamp", 50);
    Thermostat livingThermostat("Living Room Thermostat", 22.0);
    Speaker homeTheatre("Home Theatre Speaker", 40);

    Light bedsideLamp("Bedside Lamp", 30);
    Thermostat bedroomThermostat("Bedroom Thermostat", 20.0);
    Speaker bluetoothSpeaker("Bluetooth Speaker", 25);

    Light kitchenLight("Kitchen Light", 100);
    Speaker kitchenSpeaker("Kitchen Radio Speaker", 35);

    SecurityCamera frontDoorCamera("Front Door Camera");
    SecurityCamera garageCamera("Garage Camera");
    Light garageLight("Garage Light", 70);

    livingRoom.addDevice(&ceilingLight);
    livingRoom.addDevice(&floorLamp);
    livingRoom.addDevice(&livingThermostat);
    livingRoom.addDevice(&homeTheatre);

    bedroom.addDevice(&bedsideLamp);
    bedroom.addDevice(&bedroomThermostat);
    bedroom.addDevice(&bluetoothSpeaker);

    kitchen.addDevice(&kitchenLight);
    kitchen.addDevice(&kitchenSpeaker);

    garage.addDevice(&frontDoorCamera);
    garage.addDevice(&garageCamera);
    garage.addDevice(&garageLight);

    myHome.addRoom(&livingRoom);
    myHome.addRoom(&bedroom);
    myHome.addRoom(&kitchen);
    myHome.addRoom(&garage);

    std::cout << "\nSystem ready!\n";

    mainMenu(myHome);

    return 0;
}
