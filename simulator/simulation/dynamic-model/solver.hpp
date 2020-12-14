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
    Solver(DynamicModel *_pDynMod);
    void UpdateCommand(SimulationData *pSd);
    virtual void ComputeNextState(int step_ms) = 0;
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
    Euler(DynamicModel *_pDynMod) : Solver(_pDynMod){};
    void ComputeNextState(int step_ms);
    ~Euler();
};

/**
 * @brief the ComputeNextStep method is to be defined to implement the RK2 method
 * 
 */
class RungeKutta2 : public Solver
{
public:
    RungeKutta2(DynamicModel *_pDynMod) : Solver(_pDynMod){};
    void ComputeNextState(int step_ms);
};

/**
 * @brief the ComputeNextStep method is to be defined to implement the RK4 method
 * 
 */
class RungeKutta4 : public Solver
{
public:
    RungeKutta4(DynamicModel *_pDynMod) : Solver(_pDynMod){};
    void ComputeNextState(int step_ms);
};
