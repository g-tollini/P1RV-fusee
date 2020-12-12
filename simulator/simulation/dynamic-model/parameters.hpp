#pragma once

#include <eigen3/Eigen/Eigen>

/**
 * @brief 
 * 
 */
enum Model
{
    Simple = 0,
    Cardan = 1,
    Quaternions = 2
};

/**
 * @brief 
 * 
 */
enum Method
{
    methodEuler = 0,
    methodRK4 = 1
};
