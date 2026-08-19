#include "association.hpp"
#include "tracker.hpp"

Association findBestAssociation(Node* kdTree, const Point& detection, const std::vector<bool>& trackUsed) {
    Association result;
    result.trackIndex = -1;
    result.squaredDistance = std::numeric_limits<double>::max();

    if (kdTree == nullptr) {
        return result;
    }

    KDItem nearest = findBestUnusedTrack(kdTree, detection, trackUsed);

    result.trackIndex = nearest.trackIndex;

    if (nearest.trackIndex != -1) {
        result.squaredDistance = squaredDistance(detection, nearest.point);
    }

    return result;

}

std::vector<Association> greedyAssignment(const std::vector<Point>& predictedPositions, const std::vector<Point>& detections, double maxAssociationDistanceSquared) {      
    std::vector<Association> associations;

    std::vector<KDItem> items;
    std::vector<bool> tracksUsed(predictedPositions.size(),false);
    for (std::size_t i = 0; i < predictedPositions.size(); ++i) {
        items.push_back({
            predictedPositions[i],
            static_cast<int>(i)
        });
    }

    Node* root = buildKDTree(items);

    for (std::size_t detectionIndex = 0; detectionIndex < detections.size(); ++detectionIndex) {
        const Point& detection = detections[detectionIndex];

        Association association =  findBestAssociation(root, detection, tracksUsed);
        association.detectionIndex = static_cast<int>(detectionIndex);

        if (association.trackIndex != -1 && association.squaredDistance < maxAssociationDistanceSquared) {
            associations.push_back(association);

            tracksUsed[association.trackIndex] = true;
        }
    }

    deleteTree(root);

    return associations;
}

