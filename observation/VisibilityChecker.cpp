#include "VisibilityChecker.hpp"

#include <cmath>
#include <iostream>
#include <fstream>

#include "../utils/coordinates.hpp"
#include "../utils/constants.hpp"
#include "../utils/LinAlg.hpp"

// DELETE
    // std::ofstream testStream("test.txt");

VisibilityChecker::VisibilityChecker(Vector zoneECEF)
    : observationECEF(zoneECEF) {}

bool VisibilityChecker::inVisibilityZone(const Vector &satelliteECEF, double visibilityAngle)
{
    Vector projOnEllipse = projectionOnEllipse(satelliteECEF);
    Vector r = observationECEF - satelliteECEF;
    double radius = visibilityRadius(satelliteECEF, visibilityAngle, projOnEllipse);
    // get projection onto plane that's tangent to projection
    Vector zoneProjection = observationECEF * (projOnEllipse.dot(projOnEllipse) / projOnEllipse.dot(observationECEF));
    
    // use it as a direction for boundary point
    Vector onCircle = onCircleByDirection(projOnEllipse, zoneProjection, radius);

    // continue it until it touches earth
    Vector bound = boundaryPoint(onCircle, 0, satelliteECEF, projOnEllipse);
    // testStream << bound << '\n';
    double maxDistSqr = (bound - satelliteECEF).dot(bound - satelliteECEF);

    return r.dot(r) <= maxDistSqr;
}

std::vector<Vector> VisibilityChecker::boundaryPoints(int quantity, double visibilityAngle, const Vector &sat)
{
    std::vector<Vector> res;
    double angle = 2 * M_PI / quantity;

    Vector proj = projectionOnEllipse(sat);
    double radius = visibilityRadius(sat, visibilityAngle, proj);
    Vector unit = getOnCircle(proj, radius);
    // testStream << proj << '\n';
    // testStream << quantity << '\n';
    // testStream << unit << '\n';

    for (int i = 0; i < quantity; i ++) {
        res.push_back(boundaryPoint(unit, angle * i, sat, proj));
    }

    return res;
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
    double radius = h * tan(visibilityAngle);

    return radius;
}

Vector VisibilityChecker::getOnCircle(const Vector &proj, double radius)
{
    // put plane tangent to ellipsoid, take random point on it
    // => difference of this point and proj is direction to look for unit
    // unit = t * r, where t = sqrt(radius**2 / r**2)
    double X0 = proj[0], Y0 = proj[1], Z0 = proj[2];
    double x = X0, y = Y0;
    if (X0 > Y0)
        y += 5*pow(10, 5);
    else 
        x += 5*pow(10, 5);
    Vector onPlane = {x, y, Z0 - 1/Z0 * (X0*(x - X0) + Y0*(y - Y0))};
    // testStream << onPlane << '\n';

    return onCircleByDirection(proj, onPlane, radius);
}

Vector VisibilityChecker::onCircleByDirection(const Vector &proj, const Vector &onPlane, double radius) {
    Vector diff = onPlane - proj;

    double k = radius / diff.norm();

    return proj + diff * k;
}

Vector VisibilityChecker::boundaryPoint(
    const Vector &unit, double angle, const Vector &sat, const Vector &projection)
{
    Vector onCircle = LinAlg::rotateAbout(unit, projection, angle);
    // testStream << onCircle << '\n';
    Vector direction = onCircle - sat;
    Vector a = {
        Constants::Earth::MAJOR_AXIS, Constants::Earth::MAJOR_AXIS, Constants::Earth::MINOR_AXIS
    };

    double q1 = 0, q2 = 0, q3 = 0;
    for (int i = 0; i < 3; i ++) {
        q1 += pow(direction[i] / a[i], 2);
        q2 += 2 * onCircle[i] * direction[i] / (a[i] * a[i]);
        q3 += pow(onCircle[i] / a[i], 2);
    }
    q3 -= 1;
    double D = q2*q2 - 4*q1*q3;
    // taking minimal root 
    double k = (-q2 - sqrt(D)) / (2 * q1);

    return onCircle + direction * k;
}
