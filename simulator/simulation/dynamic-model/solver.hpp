#pragma once

#include "parameters.hpp"
#include "dynamic-model.hpp"

/**
 * @brief Computes the state as it will be in step_ms milliseconds
 * 
 * @param pState 
 * @param step_ms 
 */
void Solve(state_t *pState, int step_ms);

/**
 * @brief Contains the method used to solve the problem (ex : Euler, RK4, ...)
 * 
 */
class Solver
{
public:
    virtual void ComputeNextStep(DynamicModel *pDynMod, state_t *pState, int step_ms) = 0;
    Method method;

private:
};

/**
 * @brief the ComputeNextStep method is to be defined to implement Euler's method
 * 
 */
class Euler : public Solver
{
public:
    void ComputeNextStep(DynamicModel *pDynMod, state_t *pState, int step_ms);
};

/**
 * @brief the ComputeNextStep method is to be defined to implement the RK4 method
 * 
 */
class RungeKutta4 : public Solver
{
public:
    void ComputeNextStep(DynamicModel *pDynMod, state_t *pState, int step_ms);
};
