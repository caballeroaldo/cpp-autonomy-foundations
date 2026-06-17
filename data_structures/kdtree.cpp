#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

struct Point {
    double x; 
    double y;
};

struct Node {
    Point point;
    int axis;
    Node* left;
    Node* right;

    Node(const Point& p, int a): point(p), axis(a), left(nullptr), right(nullptr) {}

};

double squaredDistance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

bool compareByAxis(const Point& a, const Point& b, int axis) {
    if (axis == 0) {
        return a.x < b.x;
    }

    return a.y < b.y;
}

Node* buildKDTree(vector<Point> points, int depth = 0) {
    if (points.empty()) {
        return nullptr;
    }

    int axis = depth % 2;
    
    sort(points.begin(), points.end(), 
        [axis](const Point& a, const Point& b) {
            return compareByAxis(a, b, axis);
        });

    int mid = points.size() / 2;
    Point medianPoint = points[mid];

    Node* root = new Node(medianPoint, axis);

    vector<Point> leftPoints(points.begin(), points.begin() + mid);
    vector<Point> rightPoints(points.begin() + mid + 1, points.end());

    root->left = buildKDTree(leftPoints, depth + 1);
    root->right = buildKDTree(rightPoints, depth + 1);

    return root;
}

void printTree(Node* root, int depth = 0) {
    if (root == nullptr) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        cout << "  ";
    }

    cout << "(" << root->point.x << " , " << root->point.y << ") " << "axis=" << root->axis << "\n";

    printTree(root->left, depth + 1);
    printTree(root->right, depth + 1);
}

Point nearestNeighborHelper(Node* root, const Point& query, Point best, double& bestDist) {
    if (root == nullptr) {
        return best;
    }

    double currentDist = squaredDistance(query, root->point);
    if(currentDist < bestDist) {
        bestDist = currentDist;
        best = root->point;
    }

    int axis = root->axis;
    Node* first = nullptr;
    Node* second = nullptr;

    if( (axis == 0 && query.x < root->point.x) || 
        (axis == 1 && query.y < root->point.y)) {
            first = root->left;
            second = root->right;
    } else {
        first = root->right;
        second = root->left;
    }

    best = nearestNeighborHelper(first, query, best, bestDist);

    double axisDiff = 0;
    if(axis == 0) {
        axisDiff = query.x - root->point.x;
    } else {
        axisDiff = query.y - root->point.y;
    }

    if(axisDiff * axisDiff < bestDist) {
        best = nearestNeighborHelper(second, query, best, bestDist);
    }

    return best;
}

Point nearestNeighbor(Node* root, const Point& query) {
    if (root == nullptr) {
        cout << "Error: KD-Tree is empty\n";
        return {0,0};
    }

    Point best = root->point;
    double bestDist = squaredDistance(query, root->point);
    return nearestNeighborHelper(root, query, best, bestDist);
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {

    vector<Point> points = {
        {1, 2},
        {3, 4},
        {7, 8},
        {2, 5},
        {9, 1}
    };

    Node* root = buildKDTree(points);
    cout << "KD-Tree structure:\n";
    printTree(root);

    Point query;
    cout << "\nEnter query point (x y): ";
    cin >> query.x >> query.y;

    Point closest = nearestNeighbor(root, query);
    cout << "Closest point: (" << closest.x << ", " << closest.y << ")\n";
    double distanceToPoint = sqrt(squaredDistance(query, closest));
    cout << "Distance: " << distanceToPoint << "\n";

    deleteTree(root);

    return 0;
}