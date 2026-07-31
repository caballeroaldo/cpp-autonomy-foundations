#ifndef TRACK_STATE_HPP
#define TRACK_STATE_HPP

#include "types.hpp"
#include "kalman_filter.hpp"

struct TrackState
{
    Track track;
    KalmanFilter filter;
};

#endif