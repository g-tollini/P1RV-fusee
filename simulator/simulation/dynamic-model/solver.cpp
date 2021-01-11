#include "solver.hpp"
#include "dynamic-model.hpp"

Solver::Solver(DynamicModel *_pDynMod)
{
    pDynMod = _pDynMod;
}

void Solver::UpdateCommand(SimulationData *pSd)
{
    pDynMod->UpdateCommand(pSd);
}

void Euler::ComputeNextState(int step_ms)
{
    pDynMod->setStepms(step_ms);

    double step_fraction = 1.0;

    // The current state must be stored in pDynMod->state at this point
    pDynMod->ComputeStateDerivative();
    // This computes the current state derivative
    pDynMod->ComputeNextState(step_fraction);
}

void RungeKutta2::ComputeNextState(int step_ms)
{
    pDynMod->setStepms(step_ms);

    double step_fraction = 0.5;

    // The current state must be stored in pDynMod->state at this point
    pDynMod->ComputeStateDerivative();
    // This computes the current state derivative
    pDynMod->ComputeNextState(step_fraction);
}

void RungeKutta4::ComputeNextState(int step_ms)
{
    //RK4 method...
}