#ifndef DPS_PROJECT_SENSORS_H
#define DPS_PROJECT_SENSORS_H


#include "ThreadSafeQueue.h"

class Sensors {
public:
    explicit Sensors(ThreadSafeQueue t);
    static int start(ThreadSafeQueue t);
};


#endif //DPS_PROJECT_SENSORS_H
