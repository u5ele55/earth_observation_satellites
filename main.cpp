#include <iostream>
#include <fstream>
#include <iomanip>

#include "utils/Vector.hpp"
#include "utils/constants.hpp"
#include "utils/time.hpp"
#include "utils/coordinates.hpp"

#include "satellite_motion/modeling/RK4Solver.hpp"
#include "satellite_motion/system/SatelliteECI.hpp"
#include "satellite_motion/system/SatelliteECEF.hpp"

int main() {
    const double JD = 2460206.383;
    const double unixTimestamp = (JD - 2440587.5) * Constants::Common::SECONDS_IN_DAY;
    //ecef - 7144843.808, 217687.110, -506463.296        562.650611, -1616.516697, 7358.157263
    Vector initialPosition = {2937656.611, 14432705.729, -20838713.022};
    Vector initialSpeed = {-1356.3495006297317, 2509.5637016207775, 1545.981};
    
    Vector currentTime(7);

    SatelliteECEF *system = new SatelliteECEF(
        Constants::Earth::GEOCENTRIC_GRAVITATION_CONSTANT,
        Constants::Earth::ANGULAR_SPEED, 
        initialPosition, initialSpeed
    );
    RK4Solver solver(system, 30);

    std::ofstream trajectoryStream("trajectory.txt");

    double step = 30;
    int hour = 3600;

    for (int i = 0; i <= 1 * hour; i += step) {
        double time = i;
        Vector state = solver.solve(time);
        double x = state[1], y = state[3], z = state[5];
        long long t = i + unixTimestamp;

        currentTime = unixToTime(t);
        
        trajectoryStream << state[1] << ' ' << state[3] << ' ' << state[5] << '\n';

    }

    return 0;
}