#include <cstdlib>
#include <vector>

#include "Communication.cpp"
#include "Members.cpp"
#include "Sensors.cpp"
#include "VehicleControl.h"
#include "VehicleState.h"
#include "ThreadSafeQueue.h"

using namespace std;

int main() {
    const string version = "0.1";

    ThreadSafeQueue t;
    Communication communication(t);

    this_thread::sleep_for(5000ms);

    cout << "done" << endl;

//    Members members;
//    Sensors sensors;
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
