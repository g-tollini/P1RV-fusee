#pragma once

#include "dynamic-model.hpp"

typedef Eigen::Vector3d simple_state_t; // (z, z')
typedef double simple_command_t;        // booster_thrust

/**
 * @brief Implements the matrix equation
 * 
 */
class SimpleModel : public DynamicModel
{
public:
    SimpleModel(SimulationData *_pSd) : DynamicModel(_pSd){};
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
    simple_state_t state;
    simple_state_t dStatedt;
    simple_command_t command;
    vector<simple_state_t> buffer;
};