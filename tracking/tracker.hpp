#ifndef TRACKER_HPP
#define TRACKER_HPP
#include "kdtree.hpp"
#include "config.hpp"

void printTrackHistory(const Track& t);
void printFrameHeader(int frameNumber);
void printMatchResult(const Point& p, int trackId);
void printNewTrackResult(const Point& p, int trackId);
int findBestUnusedTrackIndex(Node* root, const Point& query, const std::vector<bool>& trackUsed);
Point predictPosition(const Track& track);
Track createTrack(int trackId, const Point& position, int frameNumber);
void updateTrack(Track& track, const Point& detection, int frameNumber, const TrackerConfig& config);

#endif