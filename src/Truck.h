//
// Created by Elbek Bakiev on 18.12.22.
//

#ifndef DPS_PROJECT_TRUCK_H
#define DPS_PROJECT_TRUCK_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>


using std::string;

const int TRUCK_ID_RANGE = 5;
const int TRUCK_NUMBER_RANGE = 10;


class Truck {
private:
    string truckId;
    string truckNumber;
    int positionX;
    int positionY;
    bool isLeader;
    const char *listOfFollowingTrucks;
    int leaderId;
    int leaderNumber;
    int speed;


public:

    string randomNumberGenerator(int range) {
        // Seed the random number generator
        sleep(1);
        srand(time(0));

        string newNumber;

        for (int i = 0; i < range; i++) {
            newNumber += std::to_string(rand() % 10 + 1);
        }

        return newNumber;

    }

    int getPosition() {
        sleep(1);

        srand(time(0));
        return rand() % 10 + 1;
    }

    void getTrackInfo() {
        std::cout << "Track ID - " << truckId << std::endl;
        std::cout << "Track Number - " << truckNumber << std::endl;
        std::cout << "Position X - " << positionX << std::endl;
        std::cout << "Position Y - " << positionY << std::endl;
    }


    Truck() {
        truckId = randomNumberGenerator(TRUCK_ID_RANGE);
        truckNumber = randomNumberGenerator(TRUCK_NUMBER_RANGE);
        positionX = getPosition();
        positionY = getPosition();

    }

};


#endif //DPS_PROJECT_TRUCK_H
