#pragma once

#include "../satellite_motion/wrapper/Satellite.hpp"

class VisibilityChecker
{
public:
    VisibilityChecker(Vector zoneECEF);
    bool inVisibilityZone(const Vector &satelliteECEF, double visibilityAngle);
    
private:
    Vector observationECEF;
private:
    Vector projectionOnEllipse(const Vector &satelliteECEF);
    double visibilityRadius(const Vector &satelliteECEF, double visibilityAngle, const Vector &projOnEl);

    Vector boundaryPoint(const Vector &unit, double angle, const Vector &sat, const Vector &projection);
};