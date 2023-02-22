#include "VehicleControl.h"

/*
 * boundary will be 600x600
 * consisting of a 500x500 box with 50 margin
 *
 * it should take about 4 seconds to traverse the area and 1s to change direction at an edge (250ms frame updates)
 * 4s * 250ms updates = 16 frames = ~30 pixels/frame
 * 1s * 250ms cornering = 4 frames = 0.5 * SPEED_LIMIT for each frame outside the boundary
 */

double SPEED_LIMIT = 20;  // pixels/frame
int BOUNDARY_T = 80;
int BOUNDARY_R = 725;
int BOUNDARY_B = 612;
int BOUNDARY_L = 140;
int INTERVAL_TIME_MS = 100;  // frame interval (update time in ms)

VehicleControl::VehicleControl(Document &vehicleModel, ThreadSafeQueue &threadSafeQueue) {
    double SPEED_X = SPEED_LIMIT * (rand() % 2 == 0 ? -1 : 1);
    double SPEED_Y = SPEED_LIMIT * (rand() % 2 == 0 ? -1 : 1);

    pair<double, double> direction = make_pair(SPEED_X, SPEED_Y);
    pair<double, double> position;

    while (true) {
        #pragma omp critical
        {
            // get the vehicleModel data
            position = make_pair(vehicleModel["x"].GetDouble(), vehicleModel["y"].GetDouble());
            double followingX = vehicleModel["following-x"].GetDouble();
            double followingY = vehicleModel["following-y"].GetDouble();

            // check if we're in a platoon and following someone
            if (followingX + followingY == 0) {
                // MANUAL MODE - not in a platoon or we are the leader
                direction = changeDirectionAtBoundary(position, direction);
            }
            else {
                // AUTONOMOUS MODE - following another vehicle
                double distance = getDistance(position.first, position.second, followingX, followingY);

                // adjust speed depending on distance
                double follow_speed = SPEED_LIMIT;
                if (distance > 120) follow_speed *= 1.4;
                if (distance < 90) follow_speed *= .6;

                // move in the direction of the leading vehicle
                double moveX = 0, moveY = 0;
                if (position.first < followingX) moveX = follow_speed;
                if (position.first > followingX) moveX = -follow_speed;
                if (position.second < followingY) moveY = follow_speed;
                if (position.second > followingY) moveY = -follow_speed;

                direction = make_pair(moveX, moveY);
            }

            // update the position
            position = move(position, direction);

            // write to the vehicleModel
            vehicleModel["x"].SetDouble(position.first);
            vehicleModel["y"].SetDouble(position.second);
            double speed = abs(direction.first) + abs(direction.second);
            vehicleModel["speed"].SetDouble(speed);
            vehicleModel["is-braking"].SetBool(speed != SPEED_LIMIT * 2);

            // add direction
            double angle = atan2(direction.second, direction.first) * 180 / M_PI;
            if (angle < 0) angle += 360;
            vehicleModel["direction"].SetInt(static_cast<int>(angle));
        }

        // tell the AWS section to publish the vehicleModel
        threadSafeQueue.push("update");

        this_thread::sleep_for(chrono::milliseconds(INTERVAL_TIME_MS));
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
    if (x < BOUNDARY_L) dx += SPEED_LIMIT * .5;
    if (y < BOUNDARY_T) dy += SPEED_LIMIT * .5;
    if (x > BOUNDARY_R) dx -= SPEED_LIMIT * .5;
    if (y > BOUNDARY_B) dy -= SPEED_LIMIT * .5;

    // enforce speed limits
    if (dx < -SPEED_LIMIT) dx = -SPEED_LIMIT;
    if (dy < -SPEED_LIMIT) dy = -SPEED_LIMIT;
    if (dx > SPEED_LIMIT) dx = SPEED_LIMIT;
    if (dy > SPEED_LIMIT) dy = SPEED_LIMIT;

    return make_pair(dx, dy);
}

// get distance between two coordinates
double VehicleControl::getDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
