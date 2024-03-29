#include <iostream>
#include <thread>

#include "Header Files/Sensors.h"


Sensors::Sensors(ThreadSafeQueue t) {
    cout << "Starting Sensors" << endl;
    thread t1(Sensors::start, t);
    // t1.join();
    t1.detach();
}

int Sensors::start(ThreadSafeQueue t) {
    for (int i = 0; i < 5; i++) {
        string s = "Sensors:" + to_string(i);
        cout << "writing - " << s << endl;
        t.push(s);
        this_thread::sleep_for(500ms);
    }
    return 0;
}
