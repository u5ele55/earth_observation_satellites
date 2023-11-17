#include "VisibilityChecker.hpp"

#include <cmath>

#include "../utils/coordinates.hpp"
#include "../utils/constants.hpp"
#include "../utils/LinAlg.hpp"

VisibilityChecker::VisibilityChecker(Vector zoneECEF)
    : observationECEF(zoneECEF) {}

bool VisibilityChecker::inVisibilityZone(const Vector &satelliteECEF, double visibilityAngle)
{
    Vector projOnEllipse = projectionOnEllipse(satelliteECEF);
    Vector r = observationECEF - projOnEllipse;

    double radius = visibilityRadius(satelliteECEF, visibilityAngle, projOnEllipse);

    return r.dot(r) <= radius * radius;
}

Vector VisibilityChecker::projectionOnEllipse(const Vector &satelliteECEF)
{
    double X = satelliteECEF[0], Y = satelliteECEF[1], Z = satelliteECEF[2];
    double a = Constants::Earth::MAJOR_AXIS, b = Constants::Earth::MINOR_AXIS;
    double t = 1 / sqrt( pow(X/a, 2) + pow(Y/a, 2) + pow(Z/b, 2) );
    Vector projection = satelliteECEF * t;

    return projection;
}

double VisibilityChecker::visibilityRadius(const Vector &satelliteECEF, double visibilityAngle, const Vector &projOnEl)
{
    Vector fromEarth = satelliteECEF - projOnEl;
    double h = fromEarth.norm();
    double radius = h * cos(visibilityAngle) / sin(visibilityAngle);

    return radius;
}

Vector VisibilityChecker::boundaryPoint(const Vector &unit, double angle, const Vector &sat, const Vector &projection)
{
    Vector onCircle = projection + LinAlg::rotateAbout(unit, projection, angle);
    Vector direction = onCircle - sat;
    Vector a = {Constants::Earth::MAJOR_AXIS, Constants::Earth::MAJOR_AXIS, Constants::Earth::MINOR_AXIS};

    double q1 = 0, q2 = 0, q3 = 0;
    for (int i = 0; i < 3; i ++) {
        q1 += pow(direction[i] / a[i], 2);
        q2 += 2 * onCircle[i] * direction[i] / (a[i] * a[i]);
        q3 += pow(onCircle[i] / a[i], 2);
    }

    double D = q2*q2 - 4*q1*q3;
    double k = (-q2 + sqrt(D)) / (2 * q1);

    return onCircle + direction * k;
}
