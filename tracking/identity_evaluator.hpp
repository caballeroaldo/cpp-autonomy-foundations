#ifndef IDENTITY_EVALUATOR_HPP
#define IDENTITY_EVALUATOR_HPP

#include <vector>

#include "ground_truth.hpp"
#include "identity_assignment.hpp"
#include "tracker.hpp"

std::vector<IdentityAssignment> buildIdentityAssignments(const std::vector<GroundTruthObject>& groundTruth, const std::vector<Track>& tracks);

int countIdentitySwitches(const std::vector<IdentityAssignment>& assignments);

std::vector<IdentityAssignment>
buildTemporalIdentityAssignments(
    const std::vector<GroundTruthObject>& groundTruth,
    const std::vector<Track>& tracks);


int countFragmentations(const std::vector<IdentityAssignment>& assignments);

double averageTrackContinuity(const std::vector<IdentityAssignment>& assignments);

double identityPreservationRate(const std::vector<IdentityAssignment>& assignments);

#endif