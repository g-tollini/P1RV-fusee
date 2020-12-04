#pragma once

#include "parameters.hpp"
#include "../../../threading.hpp"

/**
 * @brief Implements the matrix equation
 * 
 */
class DynamicModel
{
public:
    virtual void ComputeNextStep(int step_ms) = 0;
    virtual void LoadModelParameters(void) = 0;
    virtual Vec3d getPosition() = 0;
    virtual Vec3d getAttitude() = 0;
    void setStepms(int _step_ms) { step_ms = _step_ms; };

protected:
    int step_ms;
};