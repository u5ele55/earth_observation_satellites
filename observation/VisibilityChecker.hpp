#pragma once

#include "../satellite_motion/wrapper/Satellite.hpp"

class VisibilityChecker
{
public:
    VisibilityChecker(Vector zoneECEF);
    bool inVisibilityZone(Vector satelliteECEF, double visibilityAngle);
private:
    Vector observationECEF;
};