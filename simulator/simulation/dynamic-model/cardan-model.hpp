#pragma once

#include "dynamic-model.hpp"

/**
 * @brief Type of the state. May vary so define it here then only use state_t
 * 
 */
typedef Eigen::Matrix<double, 12, 1> cardan_state_t;
typedef Eigen::Matrix<double, 4, 1> cardan_command_t;
typedef Eigen::Matrix<double, 6, 1> cardan_action_t;
typedef Eigen::Matrix3d cardan_rotation_t;

class CardanModel : public DynamicModel
{
public:
    CardanModel(SimulationData *_pSd) : DynamicModel(_pSd){};
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
    cardan_state_t state;
    cardan_state_t dStatedt;
    cardan_command_t command;
    vector<cardan_state_t> buffer;
};