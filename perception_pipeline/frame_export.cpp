#include "frame_export.hpp"

#include <fstream>
#include <iostream>

void exportFrameData(
    const std::vector<FrameRecord>& frameRecords,
    const std::string& filename)
{
    std::ofstream out(filename);

    if (!out.is_open())
    {
        std::cerr << "Error: Could not write to "
                  << filename << "\n";
        return;
    }

    out << "frame,"
        << "track_id,"
        << "pred_x,pred_y,"
        << "x,y,"
        << "vel_x,vel_y,"
        << "prediction_error\n";

    for (const FrameRecord& record : frameRecords)
    {
        out
            << record.frameNumber << ","
            << record.trackId << ","

            << record.predictedPosition.x << ","
            << record.predictedPosition.y << ","

            << record.position.x << ","
            << record.position.y << ","

            << record.velocity.x << ","
            << record.velocity.y << ","

            << record.predictionError
            << "\n";
    }

    std::cout << "Exported "
              << filename
              << "\n";
}