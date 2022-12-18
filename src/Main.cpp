#include <thread>
#include <iostream>

#include "ThreadSafeQueue.h"
#include "Communication.h"
#include "Sensors.h"
#include "Truck.h"


using namespace std;

int main() {
//    const string version = "0.1";

    ThreadSafeQueue t;
    Communication communication(t);
    Sensors sensors(t);

    while (true) {
        cout << "received: " + t.front() << endl;
        t.pop();
    }


//  Uncomment and run to see the resuilt
//    Truck truck1 = Truck();
//    truck1.getTrackInfo();


//    Members members;
//    VehicleControl vehicleControl;
//    VehicleState vehicleState;
//
//    string id;
//    string memberOf;
//
//    cout << "Welcome to the D&P Truck Platooning system v" << version << endl << endl;
//    cout << "Enter your unique vehicle ID:" << endl;
//    cin >> id;
//    cout << endl << "Welcome " << id << endl << endl;
//
//    members.addMember("aa");
//    members.addMember("bb");
//    members.addMember("cc");
//    members.removeMember("bb");
//    members.describe();

    return EXIT_SUCCESS;
}
