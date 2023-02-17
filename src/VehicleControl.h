#ifndef DPS_PROJECT_VEHICLECONTROL_H
#define DPS_PROJECT_VEHICLECONTROL_H

#include "rapidjson/document.h"
#include "ThreadSafeQueue.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#define M_PI 3.14159265358979323846

using namespace rapidjson;
using namespace std;

class VehicleControl {
public:
    explicit VehicleControl(Document &vehicleModel, ThreadSafeQueue &threadSafeQueue);
private:
    pair<double, double> move(pair<double, double> position, pair<double, double> direction);
    pair<double, double> changeDirectionAtBoundary(pair<double, double> position, pair<double, double> direction);
};


#endif //DPS_PROJECT_VEHICLECONTROL_H
