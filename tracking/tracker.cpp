#include "tracker.hpp"
#include <iostream>

void printTrackHistory(const Track& t) {
    std::cout << "Track " << t.id
              << " [missed=" << t.missedFrames << "]\n";

    std::cout << "  Path: ";

    for (size_t i = 0; i < t.history.size(); i++) {
        const Observation& obs = t.history[i];

        std::cout << "F" << obs.frameNumber
                  << "(" << obs.position.x
                  << "," << obs.position.y << ")";

        if (i < t.history.size() - 1) {
            std::cout << " -> ";
        }
    }

    std::cout << "\n";
}

void printFrameHeader(int frameNumber) {
    std::cout << "\n====================================\n";
    std::cout << "Frame " << frameNumber << "\n";
    std::cout << "====================================\n";
}


void printMatchResult(const Point& p, int trackId) {
    std::cout << "[Match] (" << p.x << ", "
              << p.y << ") -> Track "
              << trackId << "\n";
}

void printNewTrackResult(const Point& p, int trackId) {
    std::cout << "[New]   (" << p.x << ", "
              << p.y << ") -> Track "
              << trackId << "\n";
}

int findBestUnusedTrackIndex(Node* root, const Point& query, const std::vector<bool>& trackUsed) {
    KDItem nearest = nearestNeighbor(root, query, trackUsed);
    return nearest.trackIndex;
}

Point predictPosition(const Track& track) {
    return {
        track.position.x + track.velocity.x,
        track.position.y + track.velocity.y
    };
}

Track createTrack(int trackId, const Point& position, int frameNumber) {
    Track track;

    track.id = trackId;
    track.position = position;
    track.velocity = {0,0};
    track.missedFrames = 0;

    track.history.push_back({frameNumber, position});

    return track;
}

void updateTrack(Track& track, const Point& detection, int frameNumber, const TrackerConfig& config) {
    Point measuredVelocity = {
        detection.x - track.position.x,
        detection.y - track.position.y

    };

    double smoothing = config.velocitySmoothing;
    double measurementWeight = 1.0 - smoothing;

    track.velocity = {
        smoothing * track.velocity.x + measurementWeight * measuredVelocity.x,
        smoothing * track.velocity.y + measurementWeight * measuredVelocity.y
    };

    track.position = detection;
    track.missedFrames = 0;

    track.history.push_back({frameNumber,detection});
}
