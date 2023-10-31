#include "SatelliteECEF.hpp"

#include <cmath>

SatelliteECEF::SatelliteECEF(double mu, double omega, const Vector &initialPosition, const Vector &initialSpeed)
    : mu(mu), omega(omega), initialState(6)
{
    for (int i = 0; i < 3; i ++) {
        initialState[2 * i] = initialSpeed[i];
        initialState[2 * i + 1] = initialPosition[i];
    }
}

void SatelliteECEF::f(Vector &state) const {
    double 
        vx = state[0], x = state[1], 
        vy = state[2], y = state[3], 
        vz = state[4], z = state[5];
    double r3 = pow(x*x + y*y + z*z, 1.5);
    double omega2 = omega * omega;

    state[0] = -mu/r3 * x - omega * x;
    state[1] = vx;

    state[2] = -mu/r3 * y - omega * y;
    state[3] = vy;

    state[4] = -mu/r3 * z - omega * z;
    state[5] = vz;
}

// [vx, x, vy, y, vz, z]
Vector SatelliteECEF::getInitialState() const {
    return initialState;
}
