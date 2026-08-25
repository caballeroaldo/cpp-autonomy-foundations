#ifndef IDENTITY_EVALUATOR_HPP
#define IDENTITY_EVALUATOR_HPP

#include <vector>

#include "ground_truth.hpp"
#include "identity_assignment.hpp"
#include "tracker.hpp"

std::vector<IdentityAssignment> buildIdentityAssignments(const std::vector<GroundTruthObject>& groundTruth, const std::vector<Track>& tracks);

#endif