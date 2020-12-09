#include "simple-model.hpp"

simple_state_t ComputeStateDerivative(simple_command_t booster_thrust)
{
    static simple_state_t out = simple_state_t::Zero();
    out(0, 0) += out(1, 0);
    out(1, 0) = booster_thrust - 9.8;
    return out;
}

void SimpleModel::ComputeNextStep(int step_ms)
{
    simple_state_t dStatedt = ComputeStateDerivative(pSd->pShm->booster_thrust);

    state(0, 0) += (step_ms / 1000.0) * state(1, 0);
    state(1, 0) += (step_ms / 1000.0) * dStatedt(1, 0);
}

void SimpleModel::LoadModelParameters(void) {}

Vector3d SimpleModel::getPosition(void)
{
    Vector3d v;
    v.x = 0.0;
    v.y = 0.0;
    v.z = state(0, 0);
    return v;
}
Vector3d SimpleModel::getAttitude(void)
{
    Vector3d v;
    return v;
}
