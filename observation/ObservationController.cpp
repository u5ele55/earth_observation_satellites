#include "ObservationController.hpp"
#include "../utils/coordinates.hpp"

#include <cmath>

#include "../utils/time.hpp"

ObservationController::ObservationController(Vector zoneBLH)
    : zoneBLH(zoneBLH), zoneECEF(blh2ecef(zoneBLH)), visibilityChecker(zoneECEF) {}

bool ObservationController::sunInclinationRestrictionSatisfied(Vector zoneBLH, double sunInclinationRestriction, Vector currentTime)
{
    int n = dayOfYear(currentTime);
    double sunInclination = 23.45 * sin((284+n)/365 * 2*M_PI);
    

}
