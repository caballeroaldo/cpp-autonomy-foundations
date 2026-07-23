#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "../tracking/tracker.hpp"
#include "../tracking/metrics.hpp"
#include "frame_loader.hpp"
#include "trajectory_export.hpp"
#include "frame_export.hpp"

using namespace std;
namespace fs = std::filesystem;

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
    vector<Track> tracks;
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

        if (tracks.empty()) {
            for (const Point& p : currentFrame) {
                Track t;
                t.id = nextTrackId++;
                t.position = p;
                t.predictedPosition = p;
                t.velocity = {0,0};
                t.missedFrames = 0;
                t.history.push_back({frameNumber, p});
                tracks.push_back(t);
                metrics.tracksCreated++;
            }

            cout << "Initialized tracks from " << filename << "\n";
            frameNumber++;
            continue;
        }

        vector<KDItem> items;
        for (int i = 0; i < static_cast<int>(tracks.size()); i++) {
            tracks[i].predictedPosition = predictPosition(tracks[i]);
            items.push_back({tracks[i].predictedPosition, i});
        }

        Node* root = buildKDTree(items);
        vector<bool> trackUsed(tracks.size(), false);

        for (const Point& p : currentFrame) {
            double minDist = numeric_limits<double>::max();
            int bestTrackIndex = -1;
            Point predicted;

            if (root != nullptr && !tracks.empty()) {
                bestTrackIndex = findBestUnusedTrackIndex(root, p, trackUsed);
                if (bestTrackIndex != -1) {
                    predicted = tracks[bestTrackIndex].predictedPosition;
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

                updateTrack(tracks[bestTrackIndex], p, predicted, predictionError, frameNumber, config);
                
                FrameRecord record;
                record.frameNumber = frameNumber;
                record.trackId = tracks[bestTrackIndex].id;
                record.predictedPosition = predicted;
                record.position = tracks[bestTrackIndex].position;
                record.velocity = tracks[bestTrackIndex].velocity;
                record.predictionError = predictionError;
                frameRecords.push_back(record);

                metrics.successfulAssociations++;
                trackUsed[bestTrackIndex] = true;

                printMatchResult(p, tracks[bestTrackIndex].id);
            } else {
                metrics.missedAssociations++;
                Track newTrack = createTrack(nextTrackId++, p, frameNumber);

                tracks.push_back(newTrack);
                metrics.tracksCreated++;
                trackUsed.push_back(true);

                printNewTrackResult(p, newTrack.id);
            }
        }

        deleteTree(root);

        for (int i = 0; i < static_cast<int>(tracks.size()); i++) {
            if (!trackUsed[i]) {
                tracks[i].missedFrames++;
            } else {
                tracks[i].missedFrames = 0;
            }
        }

        for (int i = static_cast<int>(tracks.size()) - 1; i >= 0; i--) {
            if (tracks[i].missedFrames > config.maxMissedFrames) {
                cout << "Deleting track " << tracks[i].id << " due to inactivity.\n";
                metrics.tracksDeleted++;
                tracks.erase(tracks.begin() + i);
            }
        }

        cout << "\nCurrent tracks:\n";
        for (const Track& t : tracks) {
            printTrackHistory(t);
        }

        frameNumber++;
    }

    cout << "\n====================================\n";
    cout << "Final Summary\n";
    cout << "====================================\n";

    cout << "Total tracks remaining: " << tracks.size() << "\n";
    for (const Track& t: tracks) {
        printTrackHistory(t);
    }
    printTrackerMetrics(metrics);

    exportTrackHistories(tracks, "output");
    exportFrameData(frameRecords, "output/frame_data.csv");

    return 0;
}