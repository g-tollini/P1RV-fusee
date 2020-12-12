#pragma once

#include "dynamic-model.hpp"

typedef int quaternions_state_t;
typedef int quaternions_command_t;

class QuaternionsModel : public DynamicModel
{
public:
    QuaternionsModel(SimulationData *_pSd) : DynamicModel(_pSd){};
    void ComputeStateDerivative(void);
    void ComputeNextState(void);
    void LoadModelParameters(void);

    Vector3d getPosition(void);
    Vector3d getAttitude(void);
    void UpdateCommand(void);
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