#include <cmath>

#include "cardan-model.hpp"

Eigen::Matrix<double, 6, 6> MRB(double mass)
{
    Eigen::Matrix<double, 6, 6> out = Eigen::MatrixXd::Zero(6, 6);
    out.block<3, 3>(0, 0) = mass * Eigen::MatrixXd::Identity(3, 3);
    out.block<3, 3>(3, 3) = mass / 3 * Eigen::MatrixXd::Identity(3, 3);
    out(5, 5) = mass / 10;

    return out;
}

double mass = 0;
Eigen::Matrix<double, 6, 6> m_rb = Eigen::MatrixXd::Zero(6, 6);

CardanModel::CardanModel(SharedMemory *pShm)
{
    mass = max(pShm->mass, 1.0);
    m_rb = MRB(pShm->mass);
}

inline cardan_rotation_t Rx(double angle)
{
    cardan_rotation_t dStatedt;
    dStatedt << 1.0, 0.0, 0.0, 0.0, cos(angle), -sin(angle), 0.0, sin(angle),
        cos(angle);
    return dStatedt;
}

inline cardan_rotation_t Ry(double angle)
{
    cardan_rotation_t dStatedt;
    dStatedt << cos(angle), 0.0, sin(angle), 0.0, 1.0, 0.0, -sin(angle), 0.0,
        cos(angle);
    return dStatedt;
}

inline cardan_rotation_t Rz(double angle)
{
    cardan_rotation_t dStatedt;
    dStatedt << cos(angle), -sin(angle), 0.0, sin(angle), cos(angle), 0.0, 0.0, 0.0,
        1.0;
    return dStatedt;
}

inline cardan_rotation_t S(Eigen::Vector3d const &v)
{
    cardan_rotation_t vp;
    vp << 0.0, -v(2, 0), v(1, 0), v(2, 0), 0.0, -v(0, 0), -v(1, 0), v(0, 0), 0.0;
    return vp;
}

cardan_action_t TauG(cardan_rotation_t const &j1, cardan_state_t const &state)
{
    double g = 9.8;
    Eigen::Vector3d m_weight(0.0, 0.0, -mass * g);

    cardan_action_t tauG;
    tauG.block<3, 1>(0, 0) = j1.transpose() * m_weight;
    tauG.block<3, 1>(3, 0) << 0, 0, 0;

    return tauG;
}

cardan_action_t TauATilde(cardan_state_t const &state)
{
    cardan_action_t dStatedt;

    return dStatedt;
}

cardan_action_t TauD(cardan_state_t const &state)
{
    cardan_action_t tauD;

    return tauD;
}

cardan_action_t TauC(cardan_state_t const &state, cardan_command_t const &command)
{
    cardan_action_t tauC;
    tauC.block<3, 1>(0, 0) << 0, 0, command(0);
    tauC.block<3, 1>(0, 0) = Ry(command(2)) * Rx(command(1)) * tauC.block<3, 1>(0, 0);
    Eigen::Vector3d nozzle;
    nozzle << -0.50, 0, 0;
    tauC.block<3, 1>(3, 0) = S(nozzle) * tauC.block<3, 1>(0, 0);

    return tauC;
}

Eigen::Matrix<double, 6, 6> RBCoriolisMatrix(
    const Eigen::Matrix<double, 6, 1> &nu)
{
    // Before m_C_A is computed at step n, the following elements must have been
    // computed for step n : M_RB, nu
    // M_RB is computed only 1 time which is during the initialization
    Eigen::Matrix<double, 6, 6> out;
    out.block<3, 3>(0, 0) = Eigen::MatrixXd::Zero(3, 3);
    out.block<3, 3>(0, 3) = -S(m_rb.block<3, 3>(0, 0) * nu.block<3, 1>(0, 0) +
                               m_rb.block<3, 3>(0, 3) * nu.block<3, 1>(3, 0));
    out.block<3, 3>(3, 0) = out.block<3, 3>(0, 3);
    out.block<3, 3>(3, 3) = -S(m_rb.block<3, 3>(3, 0) * nu.block<3, 1>(0, 0) +
                               m_rb.block<3, 3>(3, 3) * nu.block<3, 1>(3, 0));
    return out;
}

/*
Eigen::Matrix<double, 6, 6> RBCoriolisMatrix2(
    const Eigen::Matrix<double, 6, 1> &nu)
{
    // Before m_C_A is computed at step n, the following elements must have been
    // computed for step n : M_RB, nu
    // M_RB is computed only 1 time which is during the initialization
    Eigen::Matrix<double, 6, 6> out;
    out.block<3, 3>(0, 0) = Eigen::MatrixXd::Zero(3, 3);
    out.block<3, 3>(0, 3) = -m_mass * S(nu.block<3, 1>(0, 0));
    out.block<3, 3>(3, 0) = Eigen::MatrixXd::Zero(3, 3);
    out.block<3, 3>(3, 3) = -m_mass * S(m_r_g) * S(nu.block<3, 1>(0, 0));
    return out;
}
*/

/*
Eigen::Matrix<double, 6, 6> D(
    const Eigen::Matrix<double, 6, 1> &nu, const double &depth)
{
    // Before m_D is computed at step n, the following elements must have
    // been computed for step n : m_X_u, m_Y_v, m_Z_w, m_K_p, m_M_q, m_N_r,
    // m_X_u_u, m_Y_v_v, m_Z_w_w, m_K_p_p, m_M_q_q, m_N_r_r and their dependancies
    // m_X_u, m_Y_v, m_Z_w, m_K_p, m_M_q, m_N_r are computed only 1 time which is
    // during the initialization
    Eigen::Matrix<double, 6, 6> out = Eigen::MatrixXd::Zero(6, 6);

    return out;
}
*/

#include <iostream>

void CardanModel::ComputeStateDerivative(void)
{
    cout << "state : " << state(6, 0) << " " << state(7, 0) << " " << state(8, 0) << endl;
    cardan_rotation_t j_1 =
        Rz(state(11, 0)) * Ry(state(10, 0)) * Rx(state(9, 0));
    cardan_rotation_t j_2;

    j_2 << 1.0, sin(state(9, 0)) * tan(state(10, 0)),
        cos(state(9, 0)) * tan(state(10, 0)), 0.0, cos(state(9, 0)),
        -sin(state(9, 0)), 0, sin(state(9, 0)) / cos(state(10, 0)),
        cos(state(9, 0)) / cos(state(10, 0));

    cardan_action_t nu = state.block<6, 1>(0, 0);

    cardan_action_t tau_tilde = TauG(j_1, state) + TauC(state, command);

    dStatedt.block<6, 1>(0, 0) =
        m_rb.inverse() * (tau_tilde - RBCoriolisMatrix(nu) * nu);
    dStatedt.block<3, 1>(6, 0) = j_1 * nu.block<3, 1>(0, 0);
    dStatedt.block<3, 1>(9, 0) = j_2 * nu.block<3, 1>(3, 0);
}

void CardanModel::ComputeNextState(double step_fraction)
{
    state += step_fraction * step_ms / 1000.0 * dStatedt;
}

void CardanModel::LoadModelParameters(void)
{
    // While there is no proper loader, paramaters are hard coded
}

Vector3d CardanModel::getPosition(void)
{
    Vector3d v;
    v.x = state(6, 0);
    v.y = state(7, 0);
    v.z = state(8, 0);
    return v;
}

void CardanModel::SetPosition(Vector3d position)
{
    state.block<3, 1>(6, 0) << position.x, position.y, position.z;
}

Vector3d CardanModel::getAttitude(void)
{
    Vector3d v;
    v.x = state(9, 0);
    v.y = state(10, 0);
    v.z = state(11, 0);
    return v;
}

void CardanModel::SetAttitude(Vector3d attitude)
{
    state.block<3, 1>(9, 0) << attitude.x, attitude.y, attitude.z;
}

void CardanModel::UpdateCommand(SimulationData *pSd)
{
    command(0) = 9.8; //pSd->pShm->booster_thrust;
    command(1) = pSd->pShm->tvc_angle_1;
    command(2) = pSd->pShm->tvc_angle_2;
}

void CardanModel::BufferizeState(int bufferIndex)
{
    buffer[bufferIndex] = state;
}

void CardanModel::LoadState(int bufferIndex)
{
    state = buffer[bufferIndex];
}

void CardanModel::BufferizeStateDerivative(int bufferIndex)
{
    buffer[bufferIndex] = dStatedt;
}

void CardanModel::LoadStateDerivative(int bufferIndex)
{
    dStatedt = buffer[bufferIndex];
}

void CardanModel::ClearBuffer(void)
{
    buffer.clear();
}