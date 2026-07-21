#include <algorithm>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "../tracking/tracker.hpp"
#include "frame_loader.hpp"
#include "trajectory_export.hpp"

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
                t.velocity = {0,0};
                t.missedFrames = 0;
                t.history.push_back({frameNumber, p});
                tracks.push_back(t);
            }

            cout << "Initialized tracks from " << filename << "\n";
            frameNumber++;
            continue;
        }

        vector<KDItem> items;
        for (int i = 0; i < static_cast<int>(tracks.size()); i++) {
            Point predicted = predictPosition(tracks[i]);
            items.push_back({predicted, i});
        }

        Node* root = buildKDTree(items);
        vector<bool> trackUsed(tracks.size(), false);

        for (const Point& p : currentFrame) {
            double minDist = numeric_limits<double>::max();
            int bestTrackIndex = -1;

            if (root != nullptr && !tracks.empty()) {
                bestTrackIndex = findBestUnusedTrackIndex(root, p, trackUsed);
                if (bestTrackIndex != -1) {
                    Point predicted = predictPosition(tracks[bestTrackIndex]);
                    minDist = squaredDistance(p, predicted);
                }
            }

            if (bestTrackIndex != -1 &&
                !trackUsed[bestTrackIndex] &&
                minDist < config.maxAssociationDistanceSquared) {

                updateTrack(tracks[bestTrackIndex], p, frameNumber, config);
                
                trackUsed[bestTrackIndex] = true;

                printMatchResult(p, tracks[bestTrackIndex].id);
            } else {
                Track newTrack = createTrack(nextTrackId++, p, frameNumber);

                tracks.push_back(newTrack);
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

    exportTrackHistories(tracks, "output");

    return 0;
}