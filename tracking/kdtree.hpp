#ifndef KDTREE_HPP
#define KDTREE_HPP
#include <vector>
#include "types.hpp"

struct KDItem {
    Point point;
    int trackIndex;
};

struct Node {
    KDItem item;
    int axis;
    Node* left;
    Node* right;

    Node(const KDItem& it, int a);
};

double squaredDistance(const Point& a, const Point& b);

Node* buildKDTree(std::vector<KDItem> items, int depth = 0);
KDItem nearestNeighbor(Node* root, const Point& query, const std::vector<bool>& trackUsed);
void deleteTree(Node* root);

#endif