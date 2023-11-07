#include <iostream>
#include <fstream>
#include <iomanip>

#include "utils/Vector.hpp"
#include "utils/constants.hpp"
#include "utils/time.hpp"
#include "utils/coordinates.hpp"

#include "creators/SatelliteGroupCreator.hpp"
#include "satellite_motion/wrapper/Satellite.hpp"

int main() {
    const double JD = 2460206.383;
    const double unixTimestamp = (JD - 2440587.5) * Constants::Earth::SECONDS_IN_DAY;
    
    Vector currentTime(7);
    SatelliteGroupCreator creator("../satellites.txt");
    auto satellites = creator.create();
    std::ofstream trajectoryStream("trajectory.txt");
    trajectoryStream << satellites.size() << '\n';

    double step = 30;
    int hour = 3600;
    for (int i = 0; i <= 1 * hour; i += step) {
        double time = i;
        long long t = i + unixTimestamp;
        currentTime = unixToTime(t);

        for (auto &sat : satellites) {
            Vector pos = sat->position(time);
            trajectoryStream << pos[0] << ' ' << pos[1] << ' ' << pos[2] << '\n';
            Vector ecef = myEci2ecef(pos[0], pos[1], pos[2], currentTime);
        }
        


    }

    return 0;
}