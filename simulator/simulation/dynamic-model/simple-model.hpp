#pragma once

#include "dynamic-model.hpp"

typedef Eigen::Vector3d simple_state_t;
typedef double simple_command_t;

/**
 * @brief Implements the matrix equation
 * 
 */
class SimpleModel : public DynamicModel
{
public:
    void ComputeNextStep(int step_ms);
    void LoadModelParameters(void);

    Vec3d getPosition(void);
    Vec3d getAttitude(void);

private:
    simple_state_t state;
    simple_state_t dStatedt;
    simple_command_t command;
    int step_ms;
};