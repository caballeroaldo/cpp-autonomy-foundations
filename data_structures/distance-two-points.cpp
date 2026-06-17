#include <iostream>
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

    double dist = sqrt(dx * dx + dy * dy);
    return dist;
}

int main() {
    Point p1, p2;

    cout << "Enter x and y coor for Point 1: ";
    cin >> p1.x >> p1.y;

    cout << "Enter x and y coor for Point 2: ";
    cin >> p2.x >> p2.y;

    double distance = computeDistance(p1, p2);

    cout << "Distance between (" << p1.x << ", " << p1.y << ") and (" << p2.x << ", " << p2.y << ") is " << distance << "\n";

    return 0;

}