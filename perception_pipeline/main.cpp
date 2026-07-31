#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "../tracking/tracker.hpp"
#include "../tracking/metrics.hpp"
#include "../tracking/kalman_filter.hpp"
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
    int nextTrackId = 0;

    TrackerConfig config;
    TrackerMetrics metrics;
    std::vector<FrameRecord> frameRecords;

    // Experimentation configuration
    config.velocitySmoothing = 0.0;
    // -----------------------------

    string frameFolder = (argc > 1) ? argv[1] : "frames";
    vector<string> frameFiles = getFrameFiles(frameFolder);

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
                activeTrack.track.history.push_back({frameNumber, p});
                
                activeTrack.filter.initialize(p);
                activeTracks.push_back(activeTrack);
                
                metrics.tracksCreated++;
            }

            cout << "Initialized tracks from " << filename << "\n";
            frameNumber++;
            continue;
        }

        vector<KDItem> items;
        for (int i = 0; i < static_cast<int>(activeTracks.size()); i++) {
            activeTracks[i].filter.predict();
            activeTracks[i].track.predictedPosition = activeTracks[i].filter.position();
            items.push_back({activeTracks[i].track.predictedPosition, i});
        }

        Node* root = buildKDTree(items);
        vector<bool> trackUsed(activeTracks.size(), false);

        for (const Point& p : currentFrame) {
            double minDist = numeric_limits<double>::max();
            int bestTrackIndex = -1;
            Point predicted;

            if (root != nullptr && !activeTracks.empty()) {
                bestTrackIndex = findBestUnusedTrackIndex(root, p, trackUsed);
                if (bestTrackIndex != -1) {
                    predicted = activeTracks[bestTrackIndex].track.predictedPosition;
                    minDist = squaredDistance(p, predicted);
                }
            }

            if (bestTrackIndex != -1 &&
                !trackUsed[bestTrackIndex] &&
                minDist < config.maxAssociationDistanceSquared) {

                double predictionError = std::sqrt(squaredDistance(predicted,p));
                metrics.totalPredictionError += predictionError;
                metrics.predictionSamples++;
                metrics.maxPredictionError = std::max(metrics.maxPredictionError, predictionError);

                activeTracks[bestTrackIndex].filter.update(p);
                Point correctedPosition = activeTracks[bestTrackIndex].filter.position();
                Point correctedVelocity = activeTracks[bestTrackIndex].filter.velocity();
                // DEBUG activeTracks[bestTrackIndex].filter.printCorrection();

                recordTrackObservation(activeTracks[bestTrackIndex].track, correctedPosition, correctedVelocity, predicted, predictionError, frameNumber);
                
                FrameRecord record;
                record.frameNumber = frameNumber;
                record.trackId = activeTracks[bestTrackIndex].track.id;
                record.predictedPosition = predicted;
                record.position = activeTracks[bestTrackIndex].track.position;
                record.velocity = activeTracks[bestTrackIndex].track.velocity;
                record.predictionError = predictionError;
                frameRecords.push_back(record);

                metrics.successfulAssociations++;
                trackUsed[bestTrackIndex] = true;

                printMatchResult(p, activeTracks[bestTrackIndex].track.id);
            } else {
                metrics.missedAssociations++;
                ActiveTrack activeTrack;
                activeTrack.track = createTrack(nextTrackId++, p, frameNumber);
                activeTrack.filter.initialize(p);
                activeTracks.push_back(activeTrack);


                metrics.tracksCreated++;
                trackUsed.push_back(true);

                printNewTrackResult(p, activeTrack.track.id);
            }
        }

        deleteTree(root);

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

    vector<Track> tracks;
    for (const ActiveTrack& activeTrack : activeTracks) {
        tracks.push_back(activeTrack.track);
    }

    exportTrackHistories(tracks, "output");
    exportFrameData(frameRecords, "output/frame_data.csv");

    return 0;
}