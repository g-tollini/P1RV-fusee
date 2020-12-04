#pragma once

#include "dynamic-model.hpp"

typedef int quaternions_state_t;
typedef int quaternions_command_t;

class QuaternionsModel : public DynamicModel
{
public:
    void ComputeNextStep(int step_ms);
    void LoadModelParameters(void);

    Vec3d getPosition(void);
    Vec3d getAttitude(void);
};