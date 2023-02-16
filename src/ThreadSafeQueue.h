#ifndef DPS_PROJECT_THREADSAFEQUEUE_H
#define DPS_PROJECT_THREADSAFEQUEUE_H

#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

class ThreadSafeQueue {
public:
    void push(const string& item);
    string front();
    void pop();
};

#endif //DPS_PROJECT_THREADSAFEQUEUE_H
