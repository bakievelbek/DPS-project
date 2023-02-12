#ifndef DPS_PROJECT_COMMUNICATION_H
#define DPS_PROJECT_COMMUNICATION_H


#include "ThreadSafeQueue.h"

class Communication {
public:
    explicit Communication(ThreadSafeQueue t);
    static int start(ThreadSafeQueue t);
};


#endif //DPS_PROJECT_COMMUNICATION_H
