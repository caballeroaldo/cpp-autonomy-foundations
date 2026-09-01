#include <iostream>
#include <vector>

#include "../ground_truth_reader.hpp"
#include "../identity_evaluator.hpp"
#include "../tracker.hpp"

int main()
{
    try
    {
        auto groundTruth = loadGroundTruth(
            "frames/tmp/prediction_demo_clean/ground_truth_tracks.csv");

        std::vector<Track> tracks;

        for (const auto& object : groundTruth) {
            Track track;
            track.id = object.objectId;
            for (const auto& gtObservation : object.history) {
                Observation observation;

                observation.frameNumber =
                    gtObservation.frameNumber;

                observation.position.x =
                    gtObservation.position.x - 1;

                observation.position.y =
                    gtObservation.position.y - 1;
                /* Identity Switche Check
                if (observation.frameNumber >= 6) {
                    if (track.id == 0) {
                        observation.position.x += 170;
                        observation.position.y += 35;
                    } else if (track.id == 1) {
                        observation.position.x -= 170;
                        observation.position.y -= 35;
                    }
                }
                // */

                observation.predictedPosition =
                    observation.position;

                observation.predictionError = 0.0;

                if (track.id == 0 && observation.frameNumber >= 6 && observation.frameNumber <= 7) { 
                    continue;
                }

                track.history.push_back(observation);
            }

            tracks.push_back(track);
        }

        // Using temporalAssignments instead of the first version of Identity Assignments
        auto assignments = buildTemporalIdentityAssignments(
            groundTruth,
            tracks);

        std::cout
            << "Loaded "
            << groundTruth.size()
            << " ground truth objects.\n";

        std::cout
            << "Tracks: "
            << tracks.size()
            << "\n";

        std::cout
            << "Assignments: "
            << assignments.size()
            << "\n";
        
        for (const auto& assignment : assignments) {
            std::cout << "Object " << assignment.objectId << " -> Track " << assignment.trackId << " (Frame " << assignment.frameNumber << ")\n";
            
            
        }

        int switches = countIdentitySwitches(assignments);
        int fragmentations = countFragmentations(assignments);
        double continuity = averageTrackContinuity(assignments);
        double preservation = identityPreservationRate(assignments);

        std::cout << "Temporal assignments: " << assignments.size() << "\n";

        std::cout << "Identity Switches: " << switches << "\n";
        std::cout << "Fragmentations: " << fragmentations << "\n";
        std::cout << "Average Track Continuity: " << continuity << "\n";
        std::cout << "Identity Preservation Rate: " << preservation * 100.0 << "%\n";

    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "Exception: "
            << exception.what()
            << '\n';

        return 1;
    }

    return 0;
}