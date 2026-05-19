#ifndef TRACKER_HPP
#define TRACKER_HPP
#include "kdtree.hpp"

void printTrackHistory(const Track& t);
void printFrameHeader(int frameNumber);
void printMatchResult(const Point& p, int trackId);
void printNewTrackResult(const Point& p, int trackId);
int findBestUnusedTrackIndex(Node* root, const Point& query, const std::vector<bool>& trackUsed);

#endif