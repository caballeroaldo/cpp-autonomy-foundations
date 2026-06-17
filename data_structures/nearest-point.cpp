#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Define a struct for the points
struct Point {
    double x;
    double y;
};

// Helper functions for computing the distance between points
double computeDistance(const Point& p1, const Point& p2) {

    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    double dist = dx * dx + dy * dy;
    return dist;
}

int main() {
    vector<Point> points;

    points.push_back({1, 2});
    points.push_back({3, 4});
    points.push_back({7, 8});

    if(points.empty()) {
        cout << "No points avaiable.\n";
        return 1;
    }

    Point p1;

    cout << "Enter query point (x, y): " << "\n";
    cin >> p1.x >> p1.y;

    Point closestPoint = points[0];
    double minDist = computeDistance(p1, points[0]);

    for(const Point& p : points) {
        double dist = computeDistance(p1, p);
        cout << "Checking (" << p.x << ", " << p.y << ") -> " << dist << "\n";
        if(dist < minDist) {
            minDist = dist;
            closestPoint = p;
        }
    }

    cout << "Closest point: (" << closestPoint.x << ", " << closestPoint.y << ")\n";
    cout<< "Distance: " << sqrt(minDist) << "\n";



    return 0;
}