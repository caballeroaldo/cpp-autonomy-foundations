#include "identity_evaluator.hpp"
#include "ground_truth.hpp"

#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>

namespace
{
    constexpr double MAX_ASSIGNMENT_DISTANCE = 25.0;
    constexpr int UNASSIGNED_TRACK = -1;

    double squaredDistanceCheck(
        const Point& first,
        const Point& second)
    {
        const double dx = static_cast<double>(first.x - second.x);
        const double dy = static_cast<double>(first.y - second.y);

        return dx * dx + dy * dy;
    }

} // anonymous namespace

std::vector<IdentityAssignment> buildIdentityAssignments(const std::vector<GroundTruthObject>& groundTruth, const std::vector<Track>& tracks) {
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

int countIdentitySwitches(const std::vector<IdentityAssignment>& assignments) {
    std::map<int, std::vector<IdentityAssignment>> histories;
    int switches = 0;

    for (const auto& assignment : assignments) {
        histories[assignment.objectId].push_back(assignment);
    }

    for (const auto& [objectId, history] : histories) {
        if (history.size() < 2) {
            continue;
        }

        int previousTrack = history.front().trackId;

        for (std::size_t i = 1; i < history.size(); ++i) {

            int currentTrack = history[i].trackId;
            bool previousValid = previousTrack != UNASSIGNED_TRACK;
            bool currentValid = currentTrack != UNASSIGNED_TRACK;

            if (previousValid && currentValid && currentTrack != previousTrack) {
                ++switches;
                // std::cout << "Identity swtich detected for Object " << objectId << " at Frame " << history[i].frameNumber << "\n";
            }
            previousTrack = currentTrack;
        }
        
        
    }

    return switches;
}

std::vector<IdentityAssignment> buildTemporalIdentityAssignments(const std::vector<GroundTruthObject>& groundTruth, const std::vector<Track>& tracks) {
    int maximumFrame = 0;

    std::vector<IdentityAssignment> assignments;

        for (const auto& object : groundTruth) {
            for (const auto& observation : object.history) {
                if (observation.frameNumber > maximumFrame) {
                    maximumFrame = observation.frameNumber;
                }
            }
        }

        for (int frame = 1; frame <= maximumFrame; ++frame) {
            for (const auto& object : groundTruth) {
                const GroundTruthObservation* currentObservation = nullptr;

                for (const auto& observation : object.history) {
                    if (observation.frameNumber == frame) {
                        currentObservation = &observation;
                        break;
                    }
                }

                if (currentObservation == nullptr) {
                    continue;
                }

                double bestDistance = std::numeric_limits<double>::max();
                const Track* bestTrack = nullptr;

                for (const auto& track : tracks) {
                    const Observation* currentTrackObservation = nullptr;

                    for (const auto& observation : track.history) {
                        if (observation.frameNumber == frame) {
                            currentTrackObservation = &observation;
                            break;
                        }
                    }

                    if (currentTrackObservation == nullptr) {
                        continue;
                    }

                    double distance = squaredDistanceCheck(currentObservation->position, currentTrackObservation->position);

                    if (distance < bestDistance) {
                        bestDistance = distance;
                        bestTrack = &track;
                    }
                }

                IdentityAssignment assignment;
                assignment.objectId = object.objectId;
                assignment.frameNumber = frame;

                if (bestTrack != nullptr && bestDistance <= MAX_ASSIGNMENT_DISTANCE * MAX_ASSIGNMENT_DISTANCE) {
                    assignment.trackId = bestTrack->id;
                } else {
                    assignment.trackId = -1;
                }

                assignments.push_back(assignment);

            }

        }
    
    return assignments;
}

int countFragmentations(const std::vector<IdentityAssignment>& assignments) {
    std::map<int, std::vector<IdentityAssignment>> histories;

    for (const auto& assignment : assignments) {
        histories[assignment.objectId].push_back(assignment);
    }
    
    int fragmentations = 0;
    for (const auto& [objectId, history] : histories) {
        bool wasAssigned = false;
        bool wasUnassigned = false;

        for (const auto& assignment : history) {
            if (assignment.trackId != -1) {
                if (wasAssigned && wasUnassigned) {
                    ++fragmentations;

                    // std::cout << "Fragmentation detected at Object " << assignment.objectId << " at Frame " << assignment.frameNumber << "\n";
                    wasUnassigned = false;
                }
                wasAssigned = true;
            } else {
                if (wasAssigned) { 
                    wasUnassigned = true;
                }
            }
        }
    }

    return fragmentations;
}

double averageTrackContinuity(const std::vector<IdentityAssignment>& assignments) {
    std::map<int, std::vector<IdentityAssignment>> histories;

    for (const auto& assignment : assignments) {
        histories[assignment.objectId].push_back(assignment);
    }

    double totalContinuity = 0.0;

    for (const auto& [objectId, history] : histories) {
        int currentRun = 0;
        int longestRun = 0;
        for (const auto& assignment : history) {
            if (assignment.trackId != UNASSIGNED_TRACK) {
                ++currentRun;
                if (currentRun > longestRun) {
                    longestRun = currentRun;
                }
            } else {
                currentRun = 0;
            }
        }
        totalContinuity += longestRun;
    }

    if (histories.empty()) {
        return 0.0;
    }
    
    return totalContinuity / histories.size();
}

double identityPreservationRate(const std::vector<IdentityAssignment>& assignments, const std::unordered_map<int, int>& identityMap) {
    if (assignments.empty()) {
        return 0.0;
    }

    int correctAssignments = 0.0;
    for (const auto& assignment : assignments) {
        auto expected = identityMap.find(assignment.objectId);
        if (expected == identityMap.end()) {
            continue;
        }

        if (assignment.trackId == expected->second) {
            ++correctAssignments;
        }
    }
    return static_cast<double>(correctAssignments) / static_cast<double>(assignments.size());
}
