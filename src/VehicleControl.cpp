#include "VehicleControl.h"

/*
 * boundary will be 600x600
 * consisting of a 500x500 box with 50 margin
 *
 * it should take about 4 seconds to traverse the area and 1s to change direction at an edge
 * 4s * 250ms updates = 16 frames = ~30 pixels/frame
 * 1s * 250ms cornering = 4 frames = 0.5 * SPEED_LIMIT for each frame outside the boundary
 */

double START_X = 300;
double START_Y = 300;
double SPEED_LIMIT = 30;  // pixels/frame
double SPEED_X = -SPEED_LIMIT;
double SPEED_Y = 0;  // -SPEED_LIMIT;
int BOUNDARY_TL = 50;
int BOUNDARY_BR = 550;

VehicleControl::VehicleControl() {
    pair<double, double> position = make_pair(START_X, START_Y);
    pair<double, double> direction = make_pair(SPEED_X, SPEED_Y);

    while (true) {
        std::cout << "x:" << position.first << " y:" << position.second << "\n";

        direction = changeDirectionAtBoundary(position, direction);
        position = move(position, direction);

//        sleep(1);
        getchar();
    }
}

// apply direction movement
pair<double, double> VehicleControl::move(pair<double, double> position, pair<double, double> direction) {
    return make_pair(position.first + direction.first, position.second + direction.second);
}

// change direction when out of bounds
pair<double, double> VehicleControl::changeDirectionAtBoundary(pair<double, double> position, pair<double, double> direction) {
    double x = position.first;
    double y = position.second;
    double dx = direction.first;
    double dy = direction.second;

    // should take 4 frames to change direction
    if (x < BOUNDARY_TL) dx += SPEED_LIMIT * .5;
    if (y < BOUNDARY_TL) dy += SPEED_LIMIT * .5;
    if (x > BOUNDARY_BR) dx -= SPEED_LIMIT * .5;
    if (y > BOUNDARY_BR) dy -= SPEED_LIMIT * .5;

    // enforce speed limits
    if (dx < -SPEED_LIMIT) dx = -SPEED_LIMIT;
    if (dy < -SPEED_LIMIT) dy = -SPEED_LIMIT;
    if (dx > SPEED_LIMIT) dx = SPEED_LIMIT;
    if (dy > SPEED_LIMIT) dy = SPEED_LIMIT;

    return make_pair(dx, dy);
}
