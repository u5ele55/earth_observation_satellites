#include "SatelliteGroupCreator.hpp"

#include "../satellite_motion/system/SatelliteECI.hpp"
#include "../satellite_motion/modeling/RK4Solver.hpp"
#include "../utils/constants.hpp"

SatelliteGroupCreator::SatelliteGroupCreator(const std::string &filename)
    : file(filename) {}

std::vector<Satellite*> SatelliteGroupCreator::create()
{
    std::vector<Satellite*> res;
    int N;
    file >> N;

    Vector initialPosition(3), initialVelocity(3);
    Restrictions restrictions;

    for(int i = 0; i < N; i ++) {
        file >> initialPosition[0] >> initialPosition[1] >> initialPosition[2];
        file >> initialVelocity[0] >> initialVelocity[1] >> initialVelocity[2];
        file >> restrictions.visibilityAngle >> restrictions.sunInclinationAngle;
        SatelliteECI *system = new SatelliteECI(
            Constants::Earth::GEOCENTRIC_GRAVITATION_CONSTANT,
            Constants::Earth::ANGULAR_SPEED, 
            initialPosition, initialVelocity
        );
        RK4Solver *solver = new RK4Solver(system, 30);
        auto *satellite = new Satellite(restrictions, system, solver);
        res.push_back(satellite);
    }

    return res;
}

SatelliteGroupCreator::~SatelliteGroupCreator()
{
    file.close();
}
