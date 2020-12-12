#pragma once

#include "parameters.hpp"
#include "dynamic-model.hpp"
#include "../../../threading.hpp"

/**
 * @brief Contains the method used to solve the problem (ex : Euler, RK4, ...)
 * 
 */
class Solver
{
public:
    Solver(SharedMemory *_pShm, DynamicModel *_pDynMod);
    virtual void ComputeNextStep(int step_ms) = 0;
    Method method;

protected:
    DynamicModel *pDynMod;
    SharedMemory *pShm;
};

/**
 * @brief the ComputeNextStep method is to be defined to implement Euler's method
 * 
 */
class Euler : public Solver
{
public:
    Euler(SharedMemory *_pShm, DynamicModel *_pDynMod);
    void ComputeNextStep(int step_ms);
    ~Euler();
};

/**
 * @brief the ComputeNextStep method is to be defined to implement the RK4 method
 * 
 */
class RungeKutta4 : public Solver
{
public:
    RungeKutta4(SharedMemory *_pShm, DynamicModel *_pDynMod);
    void ComputeNextStep(int step_ms);
};
