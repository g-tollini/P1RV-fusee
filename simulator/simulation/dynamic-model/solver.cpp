#include "solver.hpp"
#include "dynamic-model.hpp"

Solver::Solver(SharedMemory *_pShm)
{
    pShm = _pShm;
}

int Solver::ReadInput(void)
{
    return pShm->step_ms;
}

void Solver::WriteOutput(Vec3d const &position, Vec3d const &attitude)
{
    memcpy(&pShm->position, &position, sizeof(Vec3d));
    memcpy(&pShm->attitude, &attitude, sizeof(Vec3d));
}
void Solver::ComputeNextStep(int step_ms)
{
    return;
}

Euler::Euler(SharedMemory *_pShm) : Solver(_pShm) {}

void Euler::ComputeNextStep(int step_ms)
{
    pDynMod->setStepms(ReadInput());
    pDynMod->ComputeNextStep(step_ms);
    WriteOutput(pDynMod->getPosition(), pDynMod->getPosition());
}

RungeKutta4::RungeKutta4(SharedMemory *_pShm) : Solver(_pShm) {}

void RungeKutta4::ComputeNextStep(int step_ms)
{
    //RK4 method...
}