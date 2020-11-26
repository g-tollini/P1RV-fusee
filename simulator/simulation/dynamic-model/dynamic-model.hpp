#pragma once

#include "parameters.hpp"

/**
 * @brief Implements the matrix equation
 * 
 */
class DynamicModel
{
public:
    virtual state_t ComputeStateDerivative(state_t *pState) = 0;
};

class CardanModel : public DynamicModel
{
public:
    state_t ComputeStateDerivative(state_t *pState);
};

class QuaternionsModel : public DynamicModel
{
public:
    state_t ComputeStateDerivative(state_t *pState);
};

/**
 * @brief Initialises the state
 * 
 * @param pState 
 */
void InitState(state_t *pState)
{
    // TODO
}