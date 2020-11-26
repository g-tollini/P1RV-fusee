#include "solver.hpp"
#include "dynamic-model/dynamic-model.hpp"

void Euler::ComputeNextStep(DynamicModel *pDynMod, state_t *pState, int step_ms)
{
    state_t dStatedt = pDynMod->ComputeStateDerivative(pState);
    *pState += dStatedt * step_ms;
}

void RungeKutta4::ComputeNextStep(DynamicModel *pDynMod, state_t *pState, int step_ms)
{
    //RK4 method...
}