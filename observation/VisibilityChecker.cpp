#include "VisibilityChecker.hpp"

#include <cmath>

#include "../utils/coordinates.hpp"
#include "../utils/constants.hpp"

VisibilityChecker::VisibilityChecker(Vector zoneBLH)
    : observationECEF(blh2ecef(zoneBLH)) {}

bool VisibilityChecker::inVisibilityZone(Vector satelliteECEF, double visibilityAngle)
{
    double X = satelliteECEF[0], Y = satelliteECEF[1], Z = satelliteECEF[2];
    double a = Constants::Earth::MAJOR_AXIS, b = Constants::Earth::MINOR_AXIS;
    double t = 1 / sqrt( pow(X/a, 2) + pow(Y/a, 2) + pow(Z/b, 2) );
    Vector projectionOnEllipse = satelliteECEF * t;

    double h = sqrt((satelliteECEF - projectionOnEllipse).dot(satelliteECEF - projectionOnEllipse));
    double radius = h * cos(visibilityAngle) / sin(visibilityAngle);

    Vector r = observationECEF - projectionOnEllipse;
    
    return r.dot(r) <= radius * radius;
}
