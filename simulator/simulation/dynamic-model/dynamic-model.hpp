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
    DynamicModel(SimulationData *_pSd) { pSd = _pSd; };
    virtual void ComputeStateDerivative(void) = 0;
    virtual void ComputeNextState(void) = 0;
    virtual void LoadModelParameters(void) = 0;
    virtual Vector3d getPosition(void) = 0;
    virtual Vector3d getAttitude(void) = 0;
    void setStepms(int _step_ms)
    {
        step_ms = (double)_step_ms;
        ClearBuffer();
    };
    virtual void UpdateCommand(void) = 0;
    virtual void BufferizeState(int bufferIndex) = 0;
    virtual void LoadState(int bufferIndex) = 0;
    virtual void BufferizeStateDerivative(int bufferIndex) = 0;
    virtual void LoadStateDerivative(int bufferIndex) = 0;

protected:
    virtual void ClearBuffer(void) = 0;
    SimulationData *pSd;
    double step_ms;
};