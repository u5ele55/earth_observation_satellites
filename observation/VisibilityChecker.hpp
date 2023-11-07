#pragma once

#include "../satellite_motion/wrapper/Satellite.hpp"

class VisibilityChecker
{
public:
    VisibilityChecker(Vector zoneBLH);
    bool inVisibilityZone(Vector satelliteECEF, double visibilityAngle);
private:
    Vector observationECEF;
};