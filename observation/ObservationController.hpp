#pragma once

#include "VisibilityChecker.hpp"

class ObservationController
{    
public:
    ObservationController(Vector zoneBLH);
    bool sunInclinationRestrictionSatisfied(Vector zoneBLH, double sunInclinationRestriction, Vector currentTime);

private:
    Vector zoneBLH;
    Vector zoneECEF;
    VisibilityChecker visibilityChecker;
};