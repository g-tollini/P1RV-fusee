#include "quaternions-model.hpp"

quaternions_state_t ComputeStateDerivative(quaternions_state_t const &state, quaternions_command_t const &command)
{
    return state;
}

void QuaternionsModel::ComputeNextStep(int step_ms) {}
void QuaternionsModel::LoadModelParameters(void) {}

Vec3d QuaternionsModel::getPosition(void)
{
    Vec3d v;
    return v;
}
Vec3d QuaternionsModel::getAttitude(void)
{
    Vec3d v;
    return v;
}