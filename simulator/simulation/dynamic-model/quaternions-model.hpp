#pragma once

#include "dynamic-model.hpp"

typedef int quaternions_state_t;
typedef int quaternions_command_t;

class QuaternionsModel : public DynamicModel
{
public:
    void ComputeStateDerivative(void);
    void ComputeNextState(double step_fraction);
    void LoadModelParameters(void);

    Vector3d getPosition(void);
    void SetPosition(Vector3d);
    Vector3d getAttitude(void);
    void SetAttitude(Vector3d);
    void UpdateCommand(SimulationData *sd);
    void BufferizeState(int bufferIndex);
    void LoadState(int bufferIndex);
    void BufferizeStateDerivative(int bufferIndex);
    void LoadStateDerivative(int bufferIndex);

private:
    void ClearBuffer(void);
    quaternions_state_t state;
    quaternions_state_t dStatedt;
    quaternions_command_t command;
    vector<quaternions_state_t> buffer;
};