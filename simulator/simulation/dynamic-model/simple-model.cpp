#include "simple-model.hpp"

simple_state_t ComputeStateDerivative(int step_ms, simple_command_t command)
{
    simple_state_t out = simple_state_t::Zero();
    static int t_ms = 0;
    if (t_ms < 5000)
    {
        out(2, 0) = command;
    }
    t_ms += step_ms;
    return out;
}

void SimpleModel::ComputeNextStep(int step_ms)
{
    simple_state_t dStatedt = ComputeStateDerivative(step_ms, command);
    state += (step_ms / 1000.0) * dStatedt;
}

void SimpleModel::LoadModelParameters(void) {}

Vec3d SimpleModel::getPosition(void)
{
    Vec3d v;
    v.x = state(0, 0);
    v.y = state(1, 0);
    v.z = state(2, 0);
    return v;
}
Vec3d SimpleModel::getAttitude(void)
{
    Vec3d v;
    return v;
}
