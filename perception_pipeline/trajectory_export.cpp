#include "trajectory_export.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void exportTrackHistories(const std::vector<Track>& tracks, const std::string& outputDir) {
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    for (const Track& t : tracks) {
        std::string filename = outputDir + "/track_" + std::to_string(t.id) + ".csv";
        std::ofstream out(filename);

        if (!out.is_open()) {
            std::cerr << "Error: Could not write to " << filename << "\n";
            continue;
        }

        out << "frame,x,y\n";
        for (const Observation& obs : t.history) {
            out << obs.frameNumber << ","
                << obs.position.x << ","
                << obs.position.y << "\n";
        }

        std::cout << "Exported " << filename << "\n";
    }
}