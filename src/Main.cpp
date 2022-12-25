#include <thread>
#include <iostream>

#include "Header Files/ThreadSafeQueue.h"
#include "Header Files/Communication.h"
#include "Header Files/Sensors.h"
#include "Truck.cpp"
#include "Header Files/Truck.h"


using namespace std;

int main() {


//    const string version = "0.1";

//    ThreadSafeQueue t;
//    Communication communication(t);
//    Sensors sensors(t);
//
//    while (true) {
//        cout << "received: " + t.front() << endl;
//        t.pop();
//    }



//  Uncomment and run to see the result
    std::thread t(&Truck::activateSystem, Truck());
    std::thread t2(&Truck::activateSystem, Truck());
    t.join();
//    t2.join();


//    Truck truck = Truck();
//    truck.activateSystem();

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
