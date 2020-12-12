#include "solver.hpp"
#include "dynamic-model.hpp"

Solver::Solver(SharedMemory *_pShm, DynamicModel *_pDynMod)
{
    pShm = _pShm;
    pDynMod = _pDynMod;
}

Euler::Euler(SharedMemory *_pShm, DynamicModel *_pDynMod) : Solver(_pShm, _pDynMod) {}

void Euler::ComputeNextStep(int step_ms)
{
    pDynMod->setStepms(step_ms);
    pDynMod->UpdateCommand();
    pDynMod->ComputeStateDerivative();
    pDynMod->ComputeNextState();

    pShm->position = pDynMod->getPosition();
    pShm->attitude = pDynMod->getPosition();
}

RungeKutta4::RungeKutta4(SharedMemory *_pShm, DynamicModel *_pDynMod) : Solver(_pShm, _pDynMod) {}

void RungeKutta4::ComputeNextStep(int step_ms)
{
    //RK4 method...
}