#include "association.hpp"
#include "tracker.hpp"

Association findBestAssociation(Node* kdTree, const Point& detection, const std::vector<bool>& trackUsed) {
    Association result;
    result.trackIndex = -1;
    result.squaredDistance = std::numeric_limits<double>::max();

    if (kdTree == nullptr) {
        return result;
    }

    result.trackIndex = findBestUnusedTrackIndex(kdTree, detection, trackUsed);

    return result;

}

