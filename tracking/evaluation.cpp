#include "evaluation.hpp"
#include "identity_evaluator.hpp"

#include <unordered_map>
#include <iostream>

constexpr int UNASSIGNED_TRACK = -1;

EvaluationReport evaluateTracking(const std::vector<GroundTruthObject>& groundTruth, const std::vector<Track>& tracks) {
    EvaluationReport report;

    auto assignments =
        buildTemporalIdentityAssignments(
            groundTruth,
            tracks);

    std::unordered_map<int, int> identityMap;

    for (const auto& assignment: assignments) {
        if (assignment.trackId == UNASSIGNED_TRACK) {
            continue;
        }

        if (identityMap.find(assignment.objectId) == identityMap.end()) {
            identityMap[assignment.objectId] = assignment.trackId;
        }
    }
    /*
    for (const auto& [objectId, trackId] : identityMap) {
        std::cout
            << "Learned: Object "
            << objectId
            << " -> Track "
            << trackId
            << '\n';
    }
    */

    report.identitySwitches =
        countIdentitySwitches(assignments);

    report.fragmentations =
        countFragmentations(assignments);

    report.averageTrackContinuity =
        averageTrackContinuity(assignments);

    report.identityPreservationRate =
        identityPreservationRate(assignments, identityMap);

    return report;
}