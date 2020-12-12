#include "simple-model.hpp"

SimpleModel::SimpleModel(SimulationData *_pSd) : DynamicModel(_pSd)
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

void SimpleModel::ComputeNextState()
{
    state += step_ms / 1000.0 * dStatedt;
    string s = "state : " + to_string(state(0, 0)) + ", " + to_string(state(1, 0)) + "\0";
    pSd->sharedBuffer->push_back(s);
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

void SimpleModel::UpdateCommand(void)
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