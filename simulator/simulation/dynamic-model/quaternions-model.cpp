#include "quaternions-model.hpp"

void QuaternionsModel::ComputeStateDerivative(void) {}

void QuaternionsModel::ComputeNextState(void) {}

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

void QuaternionsModel::UpdateCommand(void)
{
}

void QuaternionsModel::BufferizeState(int bufferIndex)
{
    buffer[bufferIndex] = state;
}

void QuaternionsModel::LoadState(int bufferIndex)
{
    state = buffer[bufferIndex];
}

void QuaternionsModel::BufferizeStateDerivative(int bufferIndex)
{
    buffer[bufferIndex] = dStatedt;
}

void QuaternionsModel::LoadStateDerivative(int bufferIndex)
{
    dStatedt = buffer[bufferIndex];
}

void QuaternionsModel::ClearBuffer(void)
{
    buffer.clear();
}