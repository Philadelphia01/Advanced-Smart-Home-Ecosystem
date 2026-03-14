#ifndef SMARTHOME_H
#define SMARTHOME_H

#include "Room.h"
#include <vector>
#include <string>
#include <iostream>

class SmartHome {
private:
    std::string homeName;
    std::vector<Room*> rooms;

public:
    SmartHome(const std::string& name) : homeName(name) {}

    void addRoom(Room* room) {
        rooms.push_back(room);
        std::cout << "Room '" << room->getName() << "' added to home '" << homeName << "'.\n";
    }

    void showAllStatus() const {
        std::cout << "\n========================================\n";
        std::cout << "  SMART HOME: " << homeName << "\n";
        std::cout << "========================================\n";
        if (rooms.empty()) {
            std::cout << "  (No rooms configured)\n";
        } else {
            for (const Room* r : rooms) {
                r->showAllStatus();
            }
        }
        std::cout << "========================================\n";
    }

    void turnAllOn() {
        std::cout << "\n=== Turning ALL devices ON in entire home ===\n";
        for (Room* r : rooms) {
            r->turnAllOn();
        }
    }

    void turnAllOff() {
        std::cout << "\n=== Turning ALL devices OFF in entire home ===\n";
        for (Room* r : rooms) {
            r->turnAllOff();
        }
    }

    const std::vector<Room*>& getRooms() const { return rooms; }

    std::string getName() const { return homeName; }
};

#endif
