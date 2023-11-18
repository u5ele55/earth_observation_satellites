#include <iostream>
#include <fstream>
#include <iomanip>

#include "utils/Vector.hpp"
#include "utils/constants.hpp"
#include "utils/time.hpp"
#include "utils/coordinates.hpp"

#include "creators/SatelliteGroupCreator.hpp"
#include "satellite_motion/wrapper/Satellite.hpp"
#include "observation/ObservationController.hpp"

int main() {
    const double JD = 2460206.583;
    const double unixTimestamp = (JD - 2440587.5) * Constants::Earth::SECONDS_IN_DAY;
    
    Vector currentTime(7);

    // Creating satellite group
    SatelliteGroupCreator creator("../satellites.txt", 10);
    auto satellites = creator.create();
    std::ofstream trajectoryStream("trajectory.txt");
    trajectoryStream << satellites.size() << '\n';

    // Input zone blh
    std::ifstream spot("../spot.txt");
    Vector zoneBLH(3);
    spot >> zoneBLH[0] >> zoneBLH[1] >> zoneBLH[2];

    ObservationController observer(zoneBLH);

    double step = 30;
    int hour = 3600;
    for (int i = 0; i <= 3 * hour; i += step) {
        double time = i;
        long long t = i + unixTimestamp;
        currentTime = unixToTime(t);

        for (int i = 0; i < satellites.size(); i ++) {
            auto &sat = satellites[i];
            Vector pos = sat->position(time);
            Vector ecef = myEci2ecef(pos[0], pos[1], pos[2], currentTime);
            // std::cout << "Sat N. " << i << '\n';
            char obs = observer.observed(ecef, sat->getRestrictions(), currentTime) ? '1' : '0';
            trajectoryStream << pos[0] << ' ' << pos[1] << ' ' << pos[2] << ' ' << obs << '\n';
            trajectoryStream << ecef[0] << ' ' << ecef[1] << ' ' << ecef[2] << ' ' << obs <<'\n'; 
        }

    }

    return 0;
}