#include <iostream>
#include <thread>

#include "Communication.h"

using namespace std;


Communication::Communication(ThreadSafeQueue t) {
    cout << "Starting Communication" << endl;
    thread t1(Communication::start, t);
    // t1.join();
    t1.detach();
}

int Communication::start(ThreadSafeQueue t) {
    for (int i = 0; i < 5; i++) {
        string s = "Communication:" + to_string(i);
        cout << "writing - " << s << endl;
        t.push(s);
        this_thread::sleep_for(500ms);
    }
    return 0;
}
