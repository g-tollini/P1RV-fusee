#include "simple-model.hpp"

SimpleModel::SimpleModel()
{
    state << 0.0, 0.0;
    dStatedt << 0.0, 0.0;
    command = 0.0;
};

void SimpleModel::ComputeStateDerivative()
{
    dStatedt(0, 0) = state(1, 0);
    dStatedt(1, 0) = command - 9.8;
}

void SimpleModel::ComputeNextState(double step_fraction)
{
    state += step_fraction * step_ms / 1000.0 * dStatedt;
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

void SimpleModel::SetPosition(Vector3d position)
{
    state(0, 0) = position.z;
}

Vector3d SimpleModel::getAttitude(void)
{
    Vector3d v;
    return v;
}

void SimpleModel::SetAttitude(Vector3d attitude)
{
    //do nothing
}
void SimpleModel::UpdateCommand(SimulationData *pSd)
{
    command = pSd->pShm->booster_thrust;
}

void SimpleModel::BufferizeState(int bufferIndex)
{
    buffer[bufferIndex] = state;
}

void SimpleModel::LoadState(int bufferIndex)
{
    state = buffer[bufferIndex];
}

void SimpleModel::BufferizeStateDerivative(int bufferIndex)
{
    buffer[bufferIndex] = dStatedt;
}

void SimpleModel::LoadStateDerivative(int bufferIndex)
{
    dStatedt = buffer[bufferIndex];
}

void SimpleModel::ClearBuffer(void)
{
    buffer.clear();
}