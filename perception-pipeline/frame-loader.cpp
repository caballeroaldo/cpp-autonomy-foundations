#include "frame-loader.hpp"
#include <fstream>
#include <iostream>

std::vector<Point> loadFrame(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open frame file: " << filename << "\n";
        return points;
    }

    double x, y;

    while (file >> x >> y) {
        points.push_back({x, y});
    }
    
     return points;
}