//
// Created by Elbek Bakiev on 17.11.22.
//
#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
#include <mutex>
#define GET_VARIABLE_NAME(Variable) (#Variable)
using namespace std;
using namespace std::chrono_literals;

int LastPosition = 1;
int ConvoiIDs[5];

int valueNextDistance_CM = 300;
int velocity_KMH = 80;

// Protects shared data from being accessed at the same time
std::mutex JobLock;


int MakeLeader(int ID) {

    std::this_thread::sleep_for(1000ms);
    std::cout << "New Leader!\n";
    ConvoiIDs[0] = ID;
    return ConvoiIDs[0];
}

int AddFollower(int ID) {
    //std::lock_guard<std::mutex> lock(JobLock);
    ConvoiIDs[LastPosition] = ID;
    LastPosition = LastPosition + 1;
    //std::lock_guard<std::mutex> unlock(JobLock);
    return ConvoiIDs[LastPosition];
}

void ConvoiInfo() {
    std::this_thread::sleep_for(2000ms);
    std::cout << "Convoi Info:\n";
    std::cout << "Leader-ID:    " << ConvoiIDs[0] << "\n";
    for (int i = 1; i < 5; ++i) {
        std::cout << "Follower" << i <<"-ID: " << ConvoiIDs[i] << "\n";
    }
}

void DrivingInfos(string name, int value) {
    std::this_thread::sleep_for(1000ms);
    std::cout << name << ": " << value << "\n";
}

int AcDecelerator() {
    //trying to keep a distance between 290cm and 300cm
    std::this_thread::sleep_for(1000ms);
    if (valueNextDistance_CM > 300 && velocity_KMH < 100) {
        velocity_KMH = velocity_KMH + 5;
    }
    if (valueNextDistance_CM < 290) {
        velocity_KMH = velocity_KMH - 5;
    }
    return velocity_KMH;
}

int DriveSimulation() {
    std::this_thread::sleep_for(3000ms);
    valueNextDistance_CM = valueNextDistance_CM + ((rand() % 100) - 50);
    std::cout << "new valueNextDistance!\n";
    return valueNextDistance_CM;
}

int main()
{
    int randomNumber;
    ConvoiIDs[0] = 102;

    while (true) {
        for (int i = 0; i < 2; ++i) {
            std::thread thConvoiInfo(ConvoiInfo);
            thConvoiInfo.join(); //waits for Thread thConvoiInfo

            randomNumber = ((rand() % 800) + 100);
            std::thread thAddFollower(AddFollower, randomNumber);
            thAddFollower.join(); //waits for Thread thMakeLeader
        }
        randomNumber = ((rand() % 800) + 100);
        std::thread thMakeLeader(MakeLeader, randomNumber);
        thMakeLeader.join(); //waits for Thread thMakeLeader
    }
    //*/


    string nextDistance = "nextDistance [cm]";
    string velocity = "velocity [km/h]";
    /*
    while(true) {
        for (int i = 0; i < 3; ++i) {
            std::thread thAcDecelerator(AcDecelerator);
            thAcDecelerator.join(); //waits for Thread thAcDecelerator

            std::thread thDrivingInfoNextDistance(DrivingInfos, nextDistance,valueNextDistance_CM);
            thDrivingInfoNextDistance.join(); //waits for Thread thDrivingInfos

            std::thread thDrivingInfoVelocity(DrivingInfos, velocity, velocity_KMH);
            thDrivingInfoVelocity.join(); //waits for Thread thDrivingInfos
        }
        std::thread thDriveSimulation(DriveSimulation);
        thDriveSimulation.join(); //waits for Thread thDriveSimulation
    }
    //*/

    //system("pause>nul");
}