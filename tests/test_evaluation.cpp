#include <iostream>

#include "../evaluation.hpp"
#include "../ground_truth_reader.hpp"

// Reuse the same synthetic track generation
// from test_identity_evaluator.cpp.

int main()
{
    auto groundTruth =
        loadGroundTruth(
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

    EvaluationReport report =
        evaluateTracking(
            groundTruth,
            tracks);

    std::cout
        << "Identity Switches: "
        << report.identitySwitches
        << '\n';

    std::cout
        << "Fragmentations: "
        << report.fragmentations
        << '\n';

    std::cout
        << "Average Track Continuity: "
        << report.averageTrackContinuity
        << '\n';

    std::cout
        << "Identity Preservation Rate: "
        << report.identityPreservationRate * 100.0
        << "%\n";
}