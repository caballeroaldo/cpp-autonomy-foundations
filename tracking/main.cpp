#include <iostream>
#include <vector>
#include <limits>
#include "tracker.hpp"

using namespace std;

int main() {
    vector<Track> tracks;
    int nextTrackId = 0;

    const double distanceThreshold = 1000; // squared distance
    const int maxMissedFrames = 3;

    int frameNumber = 1;
    int numDetections;

    cout << "How many detections in frame 1? ";
    cin >> numDetections;

    vector<Point> frame1;
    for (int i = 0; i < numDetections; i++) {
        Point p;
        cout << "Enter point " << i + 1 << " (x y): ";
        cin >> p.x >> p.y;
        frame1.push_back(p);
    }

    for (const Point& p : frame1) {
        Track t;
        t.id = nextTrackId++;
        t.position = p;
        t.missedFrames = 0;
        t.history.push_back({frameNumber, p});
        tracks.push_back(t);
    }

    frameNumber++;

    while (true) {
        cout << "How many detections in frame " << frameNumber << "? ";
        cin >> numDetections;

        if (numDetections == 0) {
            cout << "No more frames, Exiting.\n";
            break;
        }

        vector<Point> currentFrame;
        for (int i = 0; i < numDetections; i++) {
            Point p;
            cout << "Enter point " << i + 1 << " (x y): ";
            cin >> p.x >> p.y;
            currentFrame.push_back(p);
        }

        printFrameHeader(frameNumber);

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