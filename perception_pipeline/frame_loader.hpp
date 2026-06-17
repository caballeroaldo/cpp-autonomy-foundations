#ifndef FRAME_LOADER_HPP
#define FRAME_LOADER_HPP

#include <string>
#include <vector>
#include "../tracking/tracker.hpp"

std::vector<Point> loadFrame(const std::string& filename);

#endif