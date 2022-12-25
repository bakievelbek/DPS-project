//
// Created by Elbek Bakiev on 18.12.22.
//

#include "Header Files/Truck.h"
#include "map"


class Truck {
private:
    string truckId;
    string truckNumber;
    int positionX;
    int positionY;
    bool isLeader;
    const char *listOfFollowingTrucks;
    int leaderId;
    int leaderNumber;
    int speed;


public:

    string randomNumberGenerator(int range) {
        // Seed the random number generator
        sleep(1);
        srand(time(0));

        string newNumber;

        for (int i = 0; i < range; i++) {
            newNumber += std::to_string(rand() % 10 + 1);
        }

        return newNumber;

    }

    int getPosition() {
        sleep(1);

        srand(time(0));
        return rand() % 10 + 1;
    }

    void getTrackInfo() {


        std::cout << "Truck ID - " << truckId << std::endl;
        std::cout << "Truck Number - " << truckNumber << std::endl;
        std::cout << "Position X - " << positionX << std::endl;
        std::cout << "Position Y - " << positionY << std::endl;


    };


    void printOnTerminalWindow(string command) {
        std::cout << "\nTerminal window: " << command << " (" << truckId << ")\n" << std::endl;
    };

    void commands() {
        std::cout << "Choose option: " << truckId << std::endl;

        std::cout << "1 - Start Platoon" << std::endl;
        std::cout << "2 - Search for Platoon \n" << std::endl;
    }


    void searchPlatoon() {
        std::cout << "Connecting: " << truckId << std::endl;
        zmq::context_t context(2);
        zmq::socket_t socket(context, zmq::socket_type::pair);

        socket.connect("tcp://localhost:5555");

        std::cout << "Connected: " << truckId << std::endl;
        while (true) {

            zmq::message_t request(5);

            memcpy(request.data(), "Hello", 5);
            socket.send(request, zmq::send_flags::none);

            socket.recv(request, zmq::recv_flags::none);

            if (!request.empty()) {
                printOnTerminalWindow("Message received");
                std::cout << request << std::endl;


            }


        }


    }

    void startPlatoon() {

        std::cout << "Starting Platoon " << truckId << std::endl;
        zmq::context_t context(2);
        zmq::socket_t socket(context, zmq::socket_type::pair);

        try {

            socket.bind("tcp://*:5555");
            printOnTerminalWindow("Platoon initialized at port 5555");

            while (true) {
                zmq::message_t request;
                sleep(2);

                socket.recv(request, zmq::recv_flags::none);


                if (!request.empty()) {
                    printOnTerminalWindow("Message received");
                    std::cout << request << std::endl;
                    memcpy(request.data(), "World", 5);
                    socket.send(request, zmq::send_flags::none);

                } else {
                    std::cout << "Waiting for connections..." << std::endl;
                }

            }

        }
        catch (...) {

            char join;

            std::cout << "Some truck has already started a platoon, do you want to join? " << std::endl;
            std::cout << "Y - yes" << std::endl;
            std::cout << "N - no \n" << std::endl;

            scanf("%s", &join);

            if (join == 'Y') {
                searchPlatoon();
            }
        }


    }


    void *activateSystem() {
        char optionNumber;

        getTrackInfo();

        printOnTerminalWindow("System starting...");
        commands();

        scanf("%s", &optionNumber);

        if (optionNumber == '1') {
            startPlatoon();
        }

        return nullptr;
    };


    Truck() {
        truckId = randomNumberGenerator(TRUCK_ID_RANGE);
        truckNumber = randomNumberGenerator(TRUCK_NUMBER_RANGE);
        positionX = getPosition();
        positionY = getPosition();

    }

};
