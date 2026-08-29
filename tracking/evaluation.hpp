#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include <vector>

#include "ground_truth.hpp"
#include "tracker.hpp"

struct EvaluationReport {
    int identitySwitches = 0;

    int fragmentations = 0;

    double averageTrackContinuity = 0.0;

    double identityPreservationRate = 0.0;
};

EvaluationReport evaluateTracking(
    const std::vector<GroundTruthObject>& groundTruth,
    const std::vector<Track>& tracks);

#endif