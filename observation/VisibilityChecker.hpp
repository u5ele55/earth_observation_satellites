#pragma once

#include "../satellite_motion/wrapper/Satellite.hpp"

class VisibilityChecker
{
public:
    VisibilityChecker(Vector zoneECEF);
    bool inVisibilityZone(const Vector &satelliteECEF, double visibilityAngle);
    std::vector<Vector> boundaryPoints(int quantity, double visibilityAngle, const Vector &sat);
private:
    Vector observationECEF;
private:
    Vector projectionOnEllipse(const Vector &satelliteECEF);
    double visibilityRadius(const Vector &satelliteECEF, double visibilityAngle, const Vector &projOnEl);

    Vector getOnCircle(const Vector &proj, double radius);
    Vector boundaryPoint(const Vector &unit, double angle, const Vector &sat, const Vector &projection);
};