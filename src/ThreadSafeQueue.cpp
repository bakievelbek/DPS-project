#include "ThreadSafeQueue.h"

mutex this_mutex;
condition_variable this_cond_var;
queue<string> this_queue;

void ThreadSafeQueue::push(const string& item) {
    {
        lock_guard<mutex> lock(this_mutex);
        this_queue.push(item);
    }

    this_cond_var.notify_one();
}

string ThreadSafeQueue::front() {
    unique_lock<mutex> lock(this_mutex);
    this_cond_var.wait(lock, [&]{ return !this_queue.empty(); });
    return this_queue.front();
}

void ThreadSafeQueue::pop() {
    lock_guard<mutex> lock(this_mutex);
    this_queue.pop();
}
