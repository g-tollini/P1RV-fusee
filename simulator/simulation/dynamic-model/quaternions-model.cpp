#include "quaternions-model.hpp"

quaternions_state_t ComputeStateDerivative(quaternions_state_t const &state, quaternions_command_t const &command)
{
    return state;
}

void QuaternionsModel::ComputeNextStep(int step_ms) {}
void QuaternionsModel::LoadModelParameters(void) {}

Vector3d QuaternionsModel::getPosition(void)
{
    Vector3d v;
    return v;
}
Vector3d QuaternionsModel::getAttitude(void)
{
    Vector3d v;
    return v;
}