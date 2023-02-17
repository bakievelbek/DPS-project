#ifndef DPS_PROJECT_VEHICLECONTROL_H
#define DPS_PROJECT_VEHICLECONTROL_H

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

class VehicleControl {
public:
    explicit VehicleControl();
private:
    pair<double, double> move(pair<double, double> position, pair<double, double> direction);
    pair<double, double> changeDirectionAtBoundary(pair<double, double> position, pair<double, double> direction);
};


#endif //DPS_PROJECT_VEHICLECONTROL_H
