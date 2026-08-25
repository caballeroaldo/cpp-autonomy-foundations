#include "identity_evaluator.hpp"
#include "ground_truth.hpp"

#include <iostream>
#include <limits>

namespace
{

    double squaredDistanceCheck(
        const Point& first,
        const Point& second)
    {
        const double dx = static_cast<double>(first.x - second.x);
        const double dy = static_cast<double>(first.y - second.y);

        return dx * dx + dy * dy;
    }

} // anonymous namespace

std::vector<IdentityAssignment>
buildIdentityAssignments(
    const std::vector<GroundTruthObject>& groundTruth,
    const std::vector<Track>& tracks)
{
    /*
    const auto& object = groundTruth.front();
    const auto& latestGroundTruth = object.history.back();

    std::cout
    << latestGroundTruth.frameNumber
    << " ("
    << latestGroundTruth.position.x
    << ", "
    << latestGroundTruth.position.y
    << ")\n";

    for (const auto& track : tracks) {
        std::cout << "Track " << track.id << "\n";
    }
    */

    std::vector<IdentityAssignment> assignments;

    if (groundTruth.empty() || tracks.empty()) {
        return assignments;
    }

    for (const auto& object : groundTruth) {
        const auto& latestGroundTruth = object.history.back();

        double bestDistance = std::numeric_limits<double>::max();
        
        const Track* bestTrack = nullptr;

        for (const auto& track : tracks) {
            if (track.history.empty()) {
                continue;
            }

            const auto& latestTrack = track.history.back();

            double distance = squaredDistanceCheck(latestGroundTruth.position, latestTrack.position);

            if (distance < bestDistance) {
                bestDistance = distance;
                bestTrack = &track;
            }


        }

        if (bestTrack != nullptr) {
            IdentityAssignment assignment;

            assignment.objectId = object.objectId;
            assignment.trackId = bestTrack->id;
            assignment.frameNumber = latestGroundTruth.frameNumber;

            assignments.push_back(assignment);
        }
    }

    return assignments;
}