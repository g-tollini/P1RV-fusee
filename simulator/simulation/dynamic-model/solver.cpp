#include "solver.hpp"
#include "dynamic-model.hpp"

Solver::Solver(SharedMemory *_pShm, DynamicModel *_pDynMod)
{
    pShm = _pShm;
    pDynMod = _pDynMod;
}

int Solver::ReadInput(void)
{
    return pShm->step_ms;
}

void Solver::WriteOutput(Vector3d const &position, Vector3d const &attitude)
{
    memcpy(&pShm->position, &position, sizeof(Vector3d));
    memcpy(&pShm->attitude, &attitude, sizeof(Vector3d));
}

Euler::Euler(SharedMemory *_pShm, DynamicModel *_pDynMod) : Solver(_pShm, _pDynMod) {}

void Euler::ComputeNextStep(int step_ms)
{
    pDynMod->ComputeNextStep(step_ms);
    WriteOutput(pDynMod->getPosition(), pDynMod->getPosition());
}

RungeKutta4::RungeKutta4(SharedMemory *_pShm, DynamicModel *_pDynMod) : Solver(_pShm, _pDynMod) {}

void RungeKutta4::ComputeNextStep(int step_ms)
{
    //RK4 method...
}