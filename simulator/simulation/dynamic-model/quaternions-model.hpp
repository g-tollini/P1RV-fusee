#pragma once

#include "dynamic-model.hpp"

typedef int quaternions_state_t;
typedef int quaternions_command_t;

class QuaternionsModel : public DynamicModel
{
public:
    QuaternionsModel(SimulationData *_pSd) : DynamicModel(_pSd){};
    void ComputeNextStep(int step_ms);
    void LoadModelParameters(void);

    Vector3d getPosition(void);
    Vector3d getAttitude(void);
};