#include "ground_truth_reader.hpp"

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

std::vector<GroundTruthObject> loadGroundTruth(const std::string csvPath) {
    std::ifstream input(csvPath);

    if (!input.is_open()) {
        throw std::runtime_error("Unable to open ground truth file: " + csvPath);
    }

    std::map<int, GroundTruthObject> objects;

    std::string header;
    std::getline(input, header);
    // std::cout << header << "\n";

    std::string line;
    
    while (std::getline(input, line)) {
        std::stringstream stream(line);

        std::string objectIdField;
        std::string frameField;
        std::string xField;
        std::string yField;

        std::getline(stream, objectIdField, ',');
        std::getline(stream, frameField, ',');
        std::getline(stream, xField, ',');
        std::getline(stream, yField, ',');

        int objectId = std::stoi(objectIdField);
        int frame = std::stoi(frameField);
        int x = std::stoi(xField);
        int y = std::stoi(yField);

        GroundTruthObservation observation;
        observation.frameNumber = frame;
        observation.position.x = x;
        observation.position.y = y;

        objects[objectId].objectId = objectId;
        objects[objectId].history.push_back(observation);

        // std::cout << "Object " << objectId << " | Frame " << observation.frameNumber << " | (" << observation.position.x << "," << observation.position.y << ")\n";
    }

    //for (const auto& [id,object] : objects) {
    //    std::cout << "Object " << id << " has " << object.history.size() << " observations\n";
    //}

    std::vector<GroundTruthObject> result;

    for (const auto& [id, object] : objects) {
        result.push_back(object);
    }


    return result;
}