#include <iostream>
#include <omp.h>
#include <unistd.h>

#include <aws/crt/Api.h>
#include <aws/crt/StlAllocator.h>
#include <aws/crt/auth/Credentials.h>
#include <aws/crt/io/TlsOptions.h>

#include <aws/iot/MqttClient.h>

using namespace std;

pair<int, int> move(int x, int y, int dx, int dy) {
    x += dx;
    y += dy;
    return make_pair(x, y);
}

pair<int, int> changeDirectionAtBoundary(int x, int y, int dx, int dy) {
    int WIDTH = 100;
    int HEIGHT = 100;
    dx = x == 0 || x == WIDTH - 1 ? -dx : dx;
    dy = y == 0 || y == HEIGHT - 1 ? -dy : dy;
    return make_pair(dx, dy);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Please specify an ID e.g. main [name]" << "\n";
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel for default(none) shared(cout)
    for (int i = 0; i < 3; i++) {
        int id = omp_get_thread_num();
        #pragma omp critical
        cout << i << " running in thread " << id << endl;
    }
//    {
//        int id = omp_get_thread_num();
//        #pragma omp critical
//        cout << "Hello, World! from thread " << id << endl;
//    }

    int START_X = 20;
    int START_Y = 50;

    pair<int, int> position = make_pair(START_X, START_Y);
    pair<int, int> direction = make_pair(-1, -1);

    while (true) {
        cout << "x:" << position.first << " y:" << position.second << "\n";

        direction = changeDirectionAtBoundary(position.first, position.second, direction.first, direction.second);
        position = move(position.first, position.second, direction.first, direction.second);
        sleep(1);
    }
}
