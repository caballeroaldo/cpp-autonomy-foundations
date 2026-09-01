#include <iostream>

#include "../ground_truth_reader.hpp"

int main()
{
    try
    {
        auto objects = loadGroundTruth(
            "frames/tmp/prediction_demo_clean/ground_truth_tracks.csv"
        );
        std::cout << "Loaded " << objects.size() << " objects.\n";
        for (const auto& object : objects) {
            std::cout << "Object " << object.objectId << " has " << object.history.size() << " observations\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cout
            << ex.what()
            << '\n';
    }
}