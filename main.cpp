#include <iostream>

#include "utils/Vector.hpp"
#include "utils/constants.hpp"
#include "utils/time.hpp"
#include "utils/coordinates.hpp"

#include "satellite_motion/modeling/RK4Solver.hpp"
#include "satellite_motion/system/SpacecraftECI.hpp"

int main() {
    const double JD = 2460206.383;
    const double unixTimestamp = (JD - 2440587.5) * Constants::Common::SECONDS_IN_DAY;

    Vector initialPosition = {6871257.864, 0.0, 0.0};
    Vector initialSpeed = {0.0, 3810.1125727278977, 6599.308558521686};
    
    Vector currentTime(7);

    SpacecraftECI *system = new SpacecraftECI(
        Constants::Earth::GEOCENTRIC_GRAVITATION_CONSTANT,
        Constants::Earth::ANGULAR_SPEED, 
        initialPosition, initialSpeed
    );
    RK4Solver solver(system, 10);

    double step = 30;
    int hour = 3600;

    for (int i = 0; i <= 1 * hour; i += step) {
        double time = i;
        Vector state = solver.solve(time);
        double x = state[1], y = state[3], z = state[5];
        long long t = i + unixTimestamp;

        currentTime = unixToTime(t);
        Vector ecef = myEci2ecef(x,y,z, currentTime);
        
        std::cout << state[1] << ' ' << state[3] << ' ' << state[5] << '\n';
        std::cout << ecef[0] <<" "<< ecef[1] <<" "<< ecef[2] << '\n';
    }

    return 0;
}