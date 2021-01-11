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
    virtual void ComputeStateDerivative(void) = 0;
    virtual void ComputeNextState(double step_fraction) = 0;
    virtual void LoadModelParameters(void) = 0;
    virtual Vector3d getPosition(void) = 0;
    virtual void SetPosition(Vector3d) = 0;
    virtual Vector3d getAttitude(void) = 0;
    virtual void SetAttitude(Vector3d) = 0;
    void setStepms(int _step_ms)
    {
        step_ms = (double)_step_ms;
        ClearBuffer();
    };
    virtual void UpdateCommand(SimulationData *pSd) = 0;
    virtual void BufferizeState(int bufferIndex) = 0;
    virtual void LoadState(int bufferIndex) = 0;
    virtual void BufferizeStateDerivative(int bufferIndex) = 0;
    virtual void LoadStateDerivative(int bufferIndex) = 0;

protected:
    virtual void ClearBuffer(void) = 0;
    double step_ms;
};