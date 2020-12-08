#pragma once

#include "dynamic-model.hpp"

#include </usr/share/eigen-3.3.8/Eigen/Core>
#include </usr/share/eigen-3.3.8/Eigen/Dense>

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
    void ComputeNextStep(int step_ms);
    void LoadModelParameters(void);

    Vector3d getPosition(void);
    Vector3d getAttitude(void);

private:
    Eigen::Matrix<double, 12, 1> state;
    Eigen::Matrix<double, 12, 1> dStatedt;
    cardan_command_t command;
};