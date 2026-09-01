#include "kalman_filter.hpp"

#include <iostream>

KalmanFilter::KalmanFilter():
    state_(Eigen::Vector4d::Zero()),
    covariance_(Eigen::Matrix4d::Identity()),
    processNoise_(Eigen::Matrix4d::Identity()),
    measurementNoise_(Eigen::Matrix2d::Identity()),
    measurementMatrix_(Eigen::Matrix<double, 2, 4>::Zero()),
    isInitialized_(false)
{
    measurementMatrix_(0,0) = 1.0;
    measurementMatrix_(1,1) = 1.0;
}

void KalmanFilter::initialize(const Point& initialPosition) {
    state_ << 
        initialPosition.x,
        initialPosition.y,
        0.0,
        0.0;
    
    covariance_ = 100.0 * Eigen::Matrix4d::Identity();
    processNoise_ = Eigen::Matrix4d::Identity() * 0.1;
    measurementNoise_ = Eigen::Matrix2d::Identity() * 1.0;

    isInitialized_ = true;
}

void KalmanFilter::predict(double dt) {
    if (!isInitialized_ || dt <= 0.0) {
        return;
    }

    Eigen::Matrix4d F = transitionMatrix(dt);
    // Predict state
    state_ = F * state_;

    // Predict covariance
    covariance_ = F * covariance_ * F.transpose() + processNoise_;
}

void KalmanFilter::update(const Point& measurement, double dt) {
    if (!isInitialized_ || dt <= 0.0) {
        return;
    }

    Eigen::Vector2d measurementVector;
    measurementVector << measurement.x, measurement.y;

    Eigen::Vector2d innovation = measurementVector - measurementMatrix_ * state_;
    #ifdef KALMAN_DEBUG
    printInnovation(innovation);
    #endif

    Eigen::Matrix2d innovationCovariance = measurementMatrix_ * covariance_ * measurementMatrix_.transpose() + measurementNoise_;
    #ifdef KALMAN_DEBUG
    printInnovationCovariance(innovationCovariance);
    #endif

    Eigen::Matrix<double, 4, 2> kalmanGain = covariance_ * measurementMatrix_.transpose() * innovationCovariance.inverse();
    #ifdef KALMAN_DEBUG
    printKalmanGain(kalmanGain);
    #endif

    state_ = state_ + kalmanGain * innovation;

    Eigen::Matrix4d identity = Eigen::Matrix4d::Identity();
    covariance_ = (identity - kalmanGain * measurementMatrix_) * covariance_;


}

Point KalmanFilter::position() const {
    return Point{state_(0),state_(1)};
}

Point KalmanFilter::velocity() const {
    return Point{state_(2), state_(3)};
}

void KalmanFilter::printPrediction() const {
    std::cout << "\nPrediction\n";
    std::cout << "==========\n";
    std::cout << "Predicted Position : ("
              << state_(0) << ", "
              << state_(1) << ")\n";
    std::cout << "Predicted Velocity : ("
              << state_(2) << ", "
              << state_(3) << ")\n";
}

void KalmanFilter::printCorrection() const
{
    std::cout << "\nCorrection\n";
    std::cout << "==========\n";
    std::cout << "Corrected Position : ("
              << state_(0) << ", "
              << state_(1) << ")\n";
    std::cout << "Corrected Velocity : ("
              << state_(2) << ", "
              << state_(3) << ")\n";
}

void KalmanFilter::printCovariance() const {
    std::cout << "\nCovariance Matrix\n"
                << "----------------\n";
    std::cout << covariance_ << "\n";
}

void KalmanFilter::printInnovation(const Eigen::Vector2d& innovation) const {
    std::cout << "\nInnovation\n";
    std::cout << "------------\n";
    std::cout << innovation << "\n";
}

void KalmanFilter::printInnovationCovariance(const Eigen::Matrix2d& innovationCovariance) const {
    std::cout << "\nInnovation Covariance\n";
    std::cout << "---------------------\n";
    std::cout
        << innovationCovariance
        << "\n";
}

void KalmanFilter::printKalmanGain(const Eigen::Matrix<double, 4 , 2>& kalmanGain) const {
    std::cout << "\nKalman Gain\n";
    std::cout << "---------------\n";
    std::cout
        << kalmanGain
        << "\n";
}

Eigen::Matrix4d KalmanFilter::transitionMatrix(double dt) const {
    Eigen::Matrix4d F = Eigen::Matrix4d::Identity();

    F(0,2) = dt;
    F(1,3) = dt;

    return F;
}
