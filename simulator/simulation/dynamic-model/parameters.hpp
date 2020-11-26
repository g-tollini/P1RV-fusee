#pragma once

/**
 * @brief Type of the state. May vary so define it here then only use state_t
 * 
 */
typedef int state_t;

/**
 * @brief 
 * 
 */
enum Model
{
    Cardan = 0,
    Quaternions = 1
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
