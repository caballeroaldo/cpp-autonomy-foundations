#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "../tracking/tracker.hpp"
#include "frame_loader.hpp"

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

int main() {
    vector<Track> tracks;
    int nextTrackId = 0;

    const double distanceThreshold = 1000;
    const int maxMissedFrames = 3;

    vector<string> frameFiles = getFrameFiles("frames");

    if (frameFiles.empty()) {
        cerr << "No frame files found in frames/\n";
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
            items.push_back({tracks[i].position, i});
        }

        Node* root = buildKDTree(items);
        vector<bool> trackUsed(tracks.size(), false);

        for (const Point& p : currentFrame) {
            double minDist = numeric_limits<double>::max();
            int bestTrackIndex = -1;

            if (root != nullptr && !tracks.empty()) {
                bestTrackIndex = findBestUnusedTrackIndex(root, p, trackUsed);
                if (bestTrackIndex != -1) {
                    minDist = squaredDistance(p, tracks[bestTrackIndex].position);
                }
            }

            if (bestTrackIndex != -1 &&
                !trackUsed[bestTrackIndex] &&
                minDist < distanceThreshold) {

                tracks[bestTrackIndex].position = p;
                tracks[bestTrackIndex].missedFrames = 0;
                tracks[bestTrackIndex].history.push_back({frameNumber, p});
                trackUsed[bestTrackIndex] = true;

                printMatchResult(p, tracks[bestTrackIndex].id);
            } else {
                Track newTrack;
                newTrack.id = nextTrackId++;
                newTrack.position = p;
                newTrack.missedFrames = 0;
                newTrack.history.push_back({frameNumber, p});

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
            if (tracks[i].missedFrames > maxMissedFrames) {
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

    return 0;
}