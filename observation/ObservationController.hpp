#pragma once

#include "VisibilityChecker.hpp"

class ObservationController
{    
public:
    ObservationController(Vector zoneBLH, VisibilityChecker& visibilityChecker);
    bool sunInclinationRestrictionSatisfied(const Vector &zoneBLH, double sunInclinationRestriction, const Vector &currentTime);
    bool observed(const Vector &satelliteECEF, const Restrictions &restrictions, const Vector &currentTime);
private:
    Vector zoneBLH;
    Vector zoneECEF;
    VisibilityChecker visibilityChecker;
};