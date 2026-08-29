#include "evaluation.hpp"

#include "identity_evaluator.hpp"

EvaluationReport evaluateTracking(const std::vector<GroundTruthObject>& groundTruth, const std::vector<Track>& tracks) {
    EvaluationReport report;

    auto assignments =
        buildTemporalIdentityAssignments(
            groundTruth,
            tracks);

    report.identitySwitches =
        countIdentitySwitches(assignments);

    report.fragmentations =
        countFragmentations(assignments);

    report.averageTrackContinuity =
        averageTrackContinuity(assignments);

    report.identityPreservationRate =
        identityPreservationRate(assignments);

    return report;
}