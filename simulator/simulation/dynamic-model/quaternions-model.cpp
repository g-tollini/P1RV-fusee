#include "quaternions-model.hpp"

void QuaternionsModel::ComputeStateDerivative(void) {}

void QuaternionsModel::ComputeNextState(double step_fraction) {}

void QuaternionsModel::LoadModelParameters(void) {}

Vector3d QuaternionsModel::getPosition(void)
{
    Vector3d v;
    return v;
}

void QuaternionsModel::SetPosition(Vector3d position)
{
}

Vector3d QuaternionsModel::getAttitude(void)
{
    Vector3d v;
    return v;
}

void QuaternionsModel::SetAttitude(Vector3d attitude)
{
}

void QuaternionsModel::UpdateCommand(SimulationData *pSd)
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