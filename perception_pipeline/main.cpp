#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <iomanip>

#include "../tracking/tracker.hpp"
#include "../tracking/metrics.hpp"
#include "../tracking/kalman_filter.hpp"
#include "../tracking/association.hpp"
#include "../tracking/hungarian.hpp"
#include "../tracking/ground_truth_reader.hpp"
#include "../tracking/evaluation.hpp"
#include "../tracking/report_export.hpp"
#include "frame_loader.hpp"
#include "trajectory_export.hpp"
#include "frame_export.hpp"

using namespace std;
namespace fs = std::filesystem;

struct ActiveTrack {
    Track track;
    KalmanFilter filter;
};

vector<string> getFrameFiles(const string& folderPath) {
    vector<string> files;
    
    if(!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        cerr << "Error: frames folder not found: " << folderPath << "\n";
        return files;
    }

    for (const auto& entry: fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }

    sort(files.begin(), files.end());
    return files;
}

int main(int argc, char** argv) {
    vector<ActiveTrack> activeTracks;
    vector<Track> completedTracks;

    int nextTrackId = 0;

    TrackerConfig config;
    TrackerMetrics metrics;
    std::vector<FrameRecord> frameRecords;

    double totalAssociationRuntimeMs = 0.0;
    std::size_t processedAssociationFrames = 0;

    // Experimentation configuration
    config.velocitySmoothing = 0.0;
    // -----------------------------

    string frameFolder = (argc > 1) ? argv[1] : "frames";
    std::string outputDirectory = (argc > 3) ? argv[3] : "output";
    vector<string> frameFiles = getFrameFiles(frameFolder);

    if (argc > 2) {
        std::string method = argv[2];

        if (method == "greedy") {
            config.associationMethod = AssociationMethod::Greedy;
        } else if (method == "hungarian") {
            config.associationMethod = AssociationMethod::Hungarian;
        } else {
            std::cerr << "Unknown association method: " << method << "\n";
            std::cerr << "Usage:\n" << " ./perception_pipeline <frame_folder> [greedy|hungarian]\n";
            return 1;
        }
    }

    if (frameFiles.empty()) {
        cerr << "No frame files found in " << frameFolder << "\n";
        return 1;
    }

    int frameNumber = 1;

    for (const string& filename : frameFiles) {
        printFrameHeader(frameNumber);

        vector<Point> currentFrame = loadFrame(filename);

        if (currentFrame.empty()) {
            cout << "No detections in " << filename << "\n";
            frameNumber++;
            continue;
        }

        if (activeTracks.empty()) {
            for (const Point& p : currentFrame) {
                ActiveTrack activeTrack;
                activeTrack.track.id = nextTrackId++;
                activeTrack.track.position = p;
                activeTrack.track.predictedPosition = p;
                activeTrack.track.velocity = {0,0};
                activeTrack.track.missedFrames = 0;
                activeTrack.track.history.push_back({frameNumber, p, p});
                
                activeTrack.filter.initialize(p);
                activeTracks.push_back(activeTrack);
                
                metrics.tracksCreated++;
            }

            cout << "Initialized tracks from " << filename << "\n";
            frameNumber++;
            continue;
        }

        vector<Point> predictedTrackPositions;
        for (int i = 0; i < static_cast<int>(activeTracks.size()); i++) {
            activeTracks[i].filter.predict();
            activeTracks[i].track.predictedPosition = activeTracks[i].filter.position();
            predictedTrackPositions.push_back(activeTracks[i].track.predictedPosition);
        }

        
        std::vector<Association> associations;

        auto associationStart = std::chrono::high_resolution_clock::now();

        std::cout << "Assocation Method: ";
        switch (config.associationMethod) {
            case AssociationMethod::Greedy:
                std::cout<< "Greedy\n";
                associations = greedyAssignment(predictedTrackPositions, currentFrame, config.maxAssociationDistanceSquared);
                break;
            case AssociationMethod::Hungarian:
                std::cout << "Hungarian\n";
                CostMatrix costMatrix = buildCostMatrix(predictedTrackPositions, currentFrame);
                associations = hungarianAssignment(costMatrix);
                break;
        }

        auto associationEnd = std::chrono::high_resolution_clock::now();
        double associationTimeMs = std::chrono::duration<double,std::milli>(associationEnd - associationStart).count();

        totalAssociationRuntimeMs += associationTimeMs;
        processedAssociationFrames++;
        
        #ifdef HUNGARIAN_DEBUG
        std::cout << "\nHungarian Assignments\n";
        std::cout << "---------------------\n";

        for (const auto& association : associations) {
            std::cout
                << "Track "
                << association.trackIndex
                << " -> Detection "
                << association.detectionIndex
                << "\n";
        }
        std::cout << "\n";
        #endif
        std::vector<bool> trackUsed(activeTracks.size(), false);

        std::vector<bool> detectionUsed(currentFrame.size(), false);

        for (const Association& association : associations) {
            int trackIndex = association.trackIndex;
            int detectionIndex = association.detectionIndex;

            Point detection =
                currentFrame[detectionIndex];

            Point predicted =
                activeTracks[trackIndex]
                    .track
                    .predictedPosition;

            double squaredPredictionError =
                squaredDistance(
                        predicted,
                        detection);
                        
            double predictionError = std::sqrt(squaredPredictionError);

            /* 
            Metrics area 
            */
            metrics.totalPredictionError += predictionError;
            metrics.predictionSamples++;
            metrics.maxPredictionError = std::max(metrics.maxPredictionError, predictionError);
            


            activeTracks[trackIndex]
                .filter
                .update(detection);

            Point correctedPosition =
                activeTracks[trackIndex]
                    .filter
                    .position();

            Point correctedVelocity =
                activeTracks[trackIndex]
                    .filter
                    .velocity();

            recordTrackObservation(
                activeTracks[trackIndex].track,
                correctedPosition,
                correctedVelocity,
                predicted,
                predictionError,
                frameNumber);
            
            metrics.successfulAssociations++;

            /*
            Frame Record Area
            */
            FrameRecord record;
            record.frameNumber = frameNumber;
            record.trackId = activeTracks[trackIndex].track.id;
            record.predictedPosition = predicted;
            record.position = activeTracks[trackIndex].track.position;
            record.velocity = activeTracks[trackIndex].track.velocity;
            record.predictionError = predictionError;
            frameRecords.push_back(record);
            trackUsed[trackIndex] = true;
            detectionUsed[detectionIndex] = true;

            printMatchResult(
                detection,
                activeTracks[trackIndex].track.id);
        }

        for (std::size_t detectionIndex = 0; detectionIndex < detectionUsed.size(); ++detectionIndex) {
            if (detectionUsed[detectionIndex]) {
                continue;
            }

            Point detection = currentFrame[detectionIndex];

            ActiveTrack activeTrack;

            activeTrack.track =
                createTrack(
                    nextTrackId++,
                    detection,
                    frameNumber);

            activeTrack.filter.initialize(detection);

            activeTracks.push_back(activeTrack);

            metrics.tracksCreated++;

            trackUsed.push_back(true);

            printNewTrackResult(
                detection,
                activeTrack.track.id);
        }

        for (int i = 0; i < static_cast<int>(activeTracks.size()); i++) {
            if (!trackUsed[i]) {
                activeTracks[i].track.missedFrames++;
            } else {
                activeTracks[i].track.missedFrames = 0;
            }
        }

        for (int i = static_cast<int>(activeTracks.size()) - 1; i >= 0; i--) {
            if (activeTracks[i].track.missedFrames > config.maxMissedFrames) {
                cout << "Deleting track " << activeTracks[i].track.id << " due to inactivity.\n";
                metrics.tracksDeleted++;
                // Preserve the completed track for evaluation
                completedTracks.push_back(activeTracks[i].track);

                activeTracks.erase(activeTracks.begin() + i);
            }
        }

        cout << "\nCurrent tracks:\n";
        for (const ActiveTrack& activeTrack : activeTracks) {
            printTrackHistory(activeTrack.track);
        }

        frameNumber++;
    }

    cout << "\n====================================\n";
    cout << "Final Summary\n";
    cout << "====================================\n";

    cout << "Total tracks remaining: " << activeTracks.size() << "\n";
    for (const ActiveTrack& activeTrack: activeTracks) {
        printTrackHistory(activeTrack.track);
    }
    printTrackerMetrics(metrics);

    std::cout << "\n================\n";
    std::cout << "Association Metrics";
    std::cout << "\n================\n";
    std::cout << "Total Association Runtime (ms): " << totalAssociationRuntimeMs << "\n";
    std::cout << "Average Association Time (ms): " << (processedAssociationFrames > 0 ? totalAssociationRuntimeMs / processedAssociationFrames : 0.0) << "\n";

    vector<Track> allTracks = completedTracks;
    for (const ActiveTrack& activeTrack : activeTracks) {
        allTracks.push_back(activeTrack.track);
    }

    auto groundTruth = loadGroundTruth(frameFolder + "/ground_truth_tracks.csv");
    
    BenchmarkReport benchmarkReport;
    benchmarkReport.datasetName = frameFolder;
    benchmarkReport.associationMethod = config.associationMethod;
    benchmarkReport.evaluation = evaluateTracking(groundTruth,allTracks);
    benchmarkReport.trackerMetrics = metrics;
    benchmarkReport.totalAssociationRuntimeMs = totalAssociationRuntimeMs;
    benchmarkReport.processedAssociationFrames = processedAssociationFrames;

    exportEvaluationReport(benchmarkReport,outputDirectory);


    exportTrackHistories(allTracks, outputDirectory);
    exportFrameData(frameRecords, outputDirectory + "/frame_data.csv");

    return 0;
}