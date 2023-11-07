#pragma once

#include "../system/ISystem.hpp"
#include "../modeling/AbstractSolver.hpp"

struct Restrictions
{
    double visibilityAngle;
    double sunInclinationAngle;
};


class Satellite {
public:
    Satellite(Restrictions restrictions, ISystem* system, AbstractSolver* solver);
    Vector position(double time);

    ~Satellite();
private:
    Restrictions restrictions;
    ISystem* system;
    AbstractSolver* solver;
};