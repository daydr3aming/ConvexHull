#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <stack>

using namespace std;

// Function to find the orientation of three points (p, q, r)
// Returns 0 if the points are collinear, 1 if they make a clockwise turn, and -1 if counterclockwise
int orientation(pair<int, int> p, pair<int, int> q, pair<int, int> r) {
    int val = (q.second - p.second) * (r.first - q.first) -
              (q.first - p.first) * (r.second - q.second);

    if (val == 0) return 0;  // Collinear
    return (val > 0) ? 1 : -1;  // Clockwise or Counterclockwise
}

int dist(pair<int, int> p1, pair<int, int> p2) {
    return (p1.first - p2.first) * (p1.first - p2.first) +
           (p1.second - p2.second) * (p1.second - p2.second);
}

// Comparator function to sort points based on polar angle
bool polarOrder(pair<int, int> p, pair<int, int> q, const vector<pair<int, int>>& points) {
    int orientation_val = orientation(points[0], p, q);
    if (orientation_val == 0) {
        // If two points make the same angle with the pivot (interior point),
        // choose the one that is closer to the pivot
        return (dist(points[0], p) < dist(points[0], q));
    }
    return (orientation_val == -1);
}


pair<int, int> nextToTop(stack<pair<int, int>>& S) {
    pair<int, int> top = S.top();
    S.pop();
    pair<int, int> next_top = S.top();
    S.push(top);
    return next_top;
}

void GrahamScan(vector<pair<int, int>> points){
    // Find a point, P, interior to the convex hull (CH) by taking the
    // average of the coordinates of all the given points.
    int n = points.size();
    if (n < 3) return;
    int ymin = points[0].second, min = 0;
    for (int i = 1; i < n; i++){
        int y = points[i].second;
        // Pick the bottom-most or choose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y &&
            points[i].first < points[min].first))
        ymin = points[i].second, min = i;
    }
    // Translate the interior point, P, and all the others, so the interior point is at the origin.
    swap(points[0], points[min]);
    sort(points.begin() + 1, points.end(), [&points](const pair<int, int>& a, const pair<int, int>& b) {
        return polarOrder(a, b, points);
    });

    // Initialize the stack with the pivot and the first two sorted points
    stack<pair<int, int>> hull;
    hull.push(points[0]);
    hull.push(points[1]);
    hull.push(points[2]);

    // Iterate through the sorted points to build the convex hull
    for (int i = 3; i < n; i++) {
        while (orientation(nextToTop(hull), hull.top(), points[i]) != -1)
            hull.pop();
        hull.push(points[i]);
    }

    // Output the convex hull points
    while (!hull.empty()) {
        pair<int, int> p = hull.top();
        cout << "(" << p.first << ", " << p.second << ")" << endl;
        hull.pop();
    }
}

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> points(n);
    char c;
    int x, y;
    for (int i = 0; i < n; i++) {
        cin >> c >> x >> c >> y >> c;
        points[i] = make_pair(x, y);
    }
    for (auto p : points) {
        cout << p.first << " " << p.second << endl;
    }
    GrahamScan(points);
    return 0;
}

