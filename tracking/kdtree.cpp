#include "kdtree.hpp"
#include <algorithm>
#include <limits>

Node::Node(const KDItem& it, int a)
    : item(it), axis(a), left(nullptr), right(nullptr) {}

double squaredDistance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static bool compareByAxis(const KDItem& a, const KDItem& b, int axis) {
    if (axis == 0) {
        return a.point.x < b.point.x;
    }
    return a.point.y < b.point.y;
}

Node* buildKDTree(std::vector<KDItem> items, int depth) {
    if (items.empty()) {
        return nullptr;
    }

    int axis = depth % 2;

    std::sort(items.begin(), items.end(),
        [axis](const KDItem& a, const KDItem& b) {
            return compareByAxis(a, b, axis);
        });

    int mid = static_cast<int>(items.size()) / 2;
    KDItem medianItem = items[mid];

    Node* root = new Node(medianItem, axis);

    std::vector<KDItem> leftItems(items.begin(), items.begin() + mid);
    std::vector<KDItem> rightItems(items.begin() + mid + 1, items.end());

    root->left = buildKDTree(leftItems, depth + 1);
    root->right = buildKDTree(rightItems, depth + 1);

    return root;
}

static KDItem nearestNeighborHelper(Node* root,
                                    const Point& query,
                                    KDItem best,
                                    double& bestDist,
                                    const std::vector<bool>& trackUsed) {
    if (root == nullptr) {
        return best;
    }

    int currentTrackIndex = root->item.trackIndex;

    if (!trackUsed[currentTrackIndex]) {
        double currentDist = squaredDistance(query, root->item.point);
        if (currentDist < bestDist) {
            bestDist = currentDist;
            best = root->item;
        }
    }

    int axis = root->axis;
    Node* first = nullptr;
    Node* second = nullptr;

    if ((axis == 0 && query.x < root->item.point.x) ||
        (axis == 1 && query.y < root->item.point.y)) {
        first = root->left;
        second = root->right;
    } else {
        first = root->right;
        second = root->left;
    }

    best = nearestNeighborHelper(first, query, best, bestDist, trackUsed);

    double axisDiff = 0;
    if (axis == 0) {
        axisDiff = query.x - root->item.point.x;
    } else {
        axisDiff = query.y - root->item.point.y;
    }

    if (axisDiff * axisDiff < bestDist) {
        best = nearestNeighborHelper(second, query, best, bestDist, trackUsed);
    }

    return best;
}

KDItem nearestNeighbor(Node* root, const Point& query, const std::vector<bool>& trackUsed) {
    if (root == nullptr) {
        return {{0, 0}, -1};
    }

    KDItem best = {{0, 0}, -1};
    double bestDist = std::numeric_limits<double>::max();

    return nearestNeighborHelper(root, query, best, bestDist, trackUsed);
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}