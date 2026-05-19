#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;

struct Point {
    double x;
    double y;
};

struct Observation {
    int frameNumber;
    Point position;
};

struct Track {
    int id;
    Point position;
    int missedFrames;
    vector<Observation> history;
};

struct KDItem {
    Point point;
    int trackIndex;
};

struct Node {
    KDItem item;
    int axis;
    Node* left;
    Node* right;

    Node(const KDItem& it, int a)
        : item(it), axis(a), left(nullptr), right(nullptr) {}
};

double squaredDistance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

bool compareByAxis(const KDItem& a, const KDItem& b, int axis) {
    if (axis == 0) {
        return a.point.x < b.point.x;
    }

    return a.point.y < b.point.y;
}

Node* buildKDTree(vector<KDItem> items, int depth = 0) {
    if (items.empty()) {
        return nullptr;
    }

    int axis = depth % 2;

    sort(items.begin(), items.end(),
        [axis](const KDItem& a, const KDItem& b) {
            return compareByAxis(a,b,axis);
        });
    
    int mid = static_cast<int>(items.size()) / 2;

    KDItem medianItem = items[mid];

    Node* root = new Node(medianItem, axis);

    vector<KDItem> leftItems(items.begin(), items.begin() + mid);
    vector<KDItem> rightItems(items.begin() + mid + 1, items.end());

    root->left = buildKDTree(leftItems, depth + 1);
    root->right = buildKDTree(rightItems, depth + 1);

    return root;
}

KDItem nearestNeighborHelper(Node* root, const Point& query, KDItem best, double& bestDist, const vector<bool>& trackUsed) {
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

    if((axis == 0 && query.x < root->item.point.x) ||
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

KDItem nearestNeighbor(Node* root, const Point& query, const vector<bool>& trackUsed) {
    if (root == nullptr) {
        return {{0,0}, -1};
    }

    KDItem best = {{0, 0}, -1};
    double bestDist = numeric_limits<double>::max();

    return nearestNeighborHelper(root, query, best, bestDist, trackUsed);
}

int findBestUnusedTrackIndex(Node* root, const Point& query, const vector<bool>& trackUsed, double& bestDist) {
    KDItem nearest = nearestNeighbor(root, query, trackUsed);

    if (nearest.trackIndex == -1) {
        bestDist = numeric_limits<double>::max();
        return -1;
    }

    bestDist = squaredDistance(query, nearest.point);
    return nearest.trackIndex;
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void printTrackHistory(const Track& t) {
    cout << "Track " << t.id << " -> ("
         << t.position.x << ", " << t.position.y
         << "), missedFrames=" << t.missedFrames << "\n";

    cout << "  Trajectory:\n";

    for (const Observation& obs : t.history) {
        cout << "    Frame " << obs.frameNumber << ": ("
             << obs.position.x << ", "
             << obs.position.y << ")\n";
    }

    cout << "\n";
}

int main() {    
    // Initialize Tracks
    vector<Track> tracks;
    int nextTrackId = 0;

    const double distanceThreshold = 1000; // Squared Distance not 1000 pixels
    const int maxMissedFrames = 3;

    int frameNumber = 1;

    // Read the first frame
    int numDetections;
    cout << "How many detections in frame 1? ";
    cin >> numDetections;

    vector<Point> frame1;
    for (int i = 0; i < numDetections; i++) {
            Point p;
            cout << "Enter point " << i + 1 << " (x y): ";
            cin >> p.x >> p.y;
            frame1.push_back(p);
    }

    // Initialize frame1 into track
    for(const Point& p: frame1) {
        Track t;
        t.id = nextTrackId++;
        t.position = p;
        t.missedFrames = 0;
        t.history.push_back({frameNumber, p});
        tracks.push_back(t);
    }

    frameNumber++;

    // loop through detections to get additional frames
    while(true) {
        cout << "How many detections in frame " << frameNumber << "? ";
        cin >> numDetections;

        if (numDetections == 0) {
            cout << "No more frames, Exiting. \n";
            break;
        }

        vector<Point> currentFrame;
        for (int i = 0; i < numDetections; i++) {
            Point p;
            cout << "Enter point " << i + 1 << " (x y): ";
            cin >> p.x >> p.y;
            currentFrame.push_back(p);
        }

        vector<KDItem> items;
        for(int i = 0; i < static_cast<int>(tracks.size()); i++) {
            items.push_back({tracks[i].position, i});
        }
        
        Node* root = buildKDTree(items);

        vector<bool> trackUsed(tracks.size(), false);

        // match detections to tracks: greedy implementation
        for (const Point& p : currentFrame) {
            double minDist = numeric_limits<double>::max();
            int bestTrackIndex = -1;
            
            
            if (root != nullptr && !tracks.empty()) {
                bestTrackIndex = findBestUnusedTrackIndex(root, p, trackUsed, minDist);
            }
            if (bestTrackIndex != -1 && !trackUsed[bestTrackIndex] && minDist < distanceThreshold) {
                // Match to existing track
                tracks[bestTrackIndex].position = p;
                tracks[bestTrackIndex].missedFrames = 0;
                tracks[bestTrackIndex].history.push_back({frameNumber, p});
                trackUsed[bestTrackIndex] = true;
                cout << "Matched point (" << p.x << ", " << p.y << ") to Track " << tracks[bestTrackIndex].id << "\n";
            } else {
                // Create a new track
                Track newTrack;
                newTrack.id = nextTrackId++;
                newTrack.position = p;
                newTrack.missedFrames = 0;
                newTrack.history.push_back({frameNumber, p});

                tracks.push_back(newTrack);
                trackUsed.push_back(true);
                cout << "Created new Track " << newTrack.id << " for point (" << p.x << ", " << p.y << ")\n";
            }
        }

        // Memory clean up
        deleteTree(root);

        // update missedFrames
        for (int i = 0; i < tracks.size(); i++) {
            if (!trackUsed[i]) {
                tracks[i].missedFrames++;
            } else {
                tracks[i].missedFrames = 0;
            }
        }

        // delete stale tracks 
        // Checking it by looping backwards
        // Side note: Added static_cast to avoid unsigned type bug
        for (int i = static_cast<int>(tracks.size()) - 1; i >= 0; i--) {
            if (tracks[i].missedFrames > maxMissedFrames) {
                cout << "Deleting track " << tracks[i].id << " due to inactivity.\n"; 
                tracks.erase(tracks.begin() + i);
            }
        }


        cout << "\nCurrent tracks:\n";
        for (const Track& t : tracks) {
            printTrackHistory(t);
        }

        frameNumber++;
    }


    return 0;
}