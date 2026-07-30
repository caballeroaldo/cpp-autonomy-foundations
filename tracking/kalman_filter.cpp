#include "kalman_filter.hpp"

KalmanFilter::KalmanFilter():
    state_(),
    isInitialized_(false)
{
}

void KalmanFilter::initialize(const Point& initialPosition) {
    state_.x = initialPosition.x;
    state_.y = initialPosition.y;

    state_.vx = 0.0;
    state_.vy = 0.0;

    isInitialized_ = true;
}

void KalmanFilter::predict(double dt) {
    if (!isInitialized_) {
        return;
    }

    state_.x += state_.vx * dt;
    state_.y += state_.vy * dt;
}

void KalmanFilter::update(const Point& measurement, double dt) {
    if (!isInitialized_) {
        return;
    }

    state_.vx = (measurement.x - state_.x) / dt;
    state_.vy = (measurement.y - state_.y) / dt;

    state_.x = measurement.x;
    state_.y = measurement.y;
}

Point KalmanFilter::position() const {
    return Point{state_.x,state_.y};
}

Point KalmanFilter::velocity() const {
    return Point{state_.vx, state_.vy};
}
