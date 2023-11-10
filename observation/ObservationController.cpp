#include "ObservationController.hpp"
#include "../utils/coordinates.hpp"

#include <cmath>

#include "../utils/time.hpp"

ObservationController::ObservationController(Vector zoneBLH)
    : zoneBLH(zoneBLH), zoneECEF(blh2ecef(zoneBLH)), visibilityChecker(zoneECEF) {}

bool ObservationController::sunInclinationRestrictionSatisfied(const Vector &zoneBLH, double sunInclinationRestriction, const Vector &currentTime)
{
    int n = dayOfYear(currentTime);
    double sunInclination = 23.45 * sin((284+n)/365 * 2*M_PI);
    
    double latitude = zoneBLH[0];
    double longitude = zoneBLH[1];
    double time = currentTime[3] + currentTime[4] / 60 + currentTime[5] / 3600;
    double hourAngle = M_PI / 12 * (time - 12) + longitude;
    

    double horizontAngle = asin(
        cos(hourAngle) * cos(latitude) * cos(sunInclination) + sin(latitude) * sin(sunInclination)
    );

    return horizontAngle >= sunInclinationRestriction;
}

bool ObservationController::observed(const Vector &satelliteECEF, const Restrictions &restrictions, const Vector &currentTime)
{
    if (!sunInclinationRestrictionSatisfied(zoneBLH, restrictions.sunInclinationAngle, currentTime))
        return false;
    
    return visibilityChecker.inVisibilityZone(satelliteECEF, restrictions.visibilityAngle);
}
