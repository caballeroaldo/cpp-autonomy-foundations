#ifndef GROUND_TRUTH_READER_HPP
#define GROUND_TRUTH_READER_HPP

#include <string>
#include <vector>

#include "ground_truth.hpp"

std::vector<GroundTruthObject> loadGroundTruth(const std::string csvPath);


#endif