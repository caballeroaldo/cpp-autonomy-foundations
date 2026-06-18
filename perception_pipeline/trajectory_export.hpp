#ifndef TRAJECTORY_EXPORT_HPP
#define TRAJECTORY_EXPORT_HPP

#include <string>
#include <vector>
#include "../tracking/types.hpp"

void exportTrackHistories(const std::vector<Track>& tracks, const std::string& outputDir);

#endif

