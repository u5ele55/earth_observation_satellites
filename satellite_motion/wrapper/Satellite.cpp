#include "Satellite.hpp"

Satellite::Satellite(Restrictions restrictions, ISystem *system, AbstractSolver *solver)
    : restrictions(restrictions), system(system), solver(solver) {}

Vector Satellite::position(double time)
{
    Vector state = solver->solve(time);
    Vector position = {state[1], state[3], state[5]};
    return position;
}

Restrictions Satellite::getRestrictions() const
{
    return restrictions;
}

Satellite::~Satellite()
{
    delete solver;
    delete system;
}
