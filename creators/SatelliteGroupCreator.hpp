#pragma once

#include <fstream>
#include "../satellite_motion/wrapper/Satellite.hpp"

class SatelliteGroupCreator
{
public:
    SatelliteGroupCreator(const std::string& filename);
    std::vector<Satellite*> create();
    ~SatelliteGroupCreator();
private:
    std::ifstream file;
};