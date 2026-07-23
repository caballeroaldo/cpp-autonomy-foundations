#ifndef FRAME_EXPORT_HPP
#define FRAME_EXPORT_HPP

#include <string>
#include <vector>

#include "../tracking/types.hpp"

struct FrameRecord{
    int frameNumber;

    int trackId;

    Point predictedPosition;
    Point position;
    Point velocity;

    double predictionError;
};

void exportFrameData(
    const std::vector<FrameRecord>& frameRecords,
    const std::string& filename
);

#endif