#include "kalman_filter.hpp"

KalmanFilter::KalmanFilter():
    state_(),
    isInitialized_(false)
{
}

void KalmanFilter::initialize(const Point& initialPosition) {
    state_ << 
        initialPosition.x,
        initialPosition.y,
        0.0,
        0.0;

    isInitialized_ = true;
}

void KalmanFilter::predict(double dt) {
    if (!isInitialized_ || dt <= 0.0) {
        return;
    }

    state_ = transitionMatrix(dt) * state_;
}

void KalmanFilter::update(const Point& measurement, double dt) {
    if (!isInitialized_ || dt <= 0.0) {
        return;
    }

    state_(2) = (measurement.x - state_(0)) / dt;
    state_(3) = (measurement.y - state_(1)) / dt;

    state_(0) = measurement.x;
    state_(1) = measurement.y;
}

Point KalmanFilter::position() const {
    return Point{state_(0),state_(1)};
}

Point KalmanFilter::velocity() const {
    return Point{state_(2), state_(3)};
}

Eigen::Matrix4d KalmanFilter::transitionMatrix(double dt) const {
    Eigen::Matrix4d F = Eigen::Matrix4d::Identity();

    F(0,2) = dt;
    F(1,3) = dt;

    return F;
}
