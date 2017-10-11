#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

/**
    A class containing an x-coordinate and a y-coordinate to simulate
    a point. 
*/
class Point {
public:
    double x, y;

    /**
        Creates a point
    */
    Point() {
    }

    /**
        Creates a point
        @param x The x-coordinate
        @param y The y-coordinate
    */
    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }

    /**
        Prints the point on cout on the format "(x, y)\n"
    */
    void print() {
        cout << "(" << this->x << ", " << this->y << ")" << endl;
    }

    /**
        Calculates the euclidian distance between this point and another
        @param p The other point
        @return The distance between this point and the provided one
    */
    long distance(Point p) {
        return (long) (sqrt( pow(this->x - p.x, 2) + pow(this->y - p.y, 2)) + 0.5L);
    }
};

/**
    Prints an array using a new line for every index

    @param arr The array to be printed
    @param size The length of the array
*/
void print_result(long* arr, int size) {
    for (int i=0; i < size; i++)
        cout << arr[i] << endl;
}

/**
    Reads two doubles from cin and returns a Point containing the values

    @return A point containing values read from cin
*/
Point readPoint() {
    double x, y;
    cin >> x >> y;
    Point a(x, y);
    return a;
}

/**
    Returns the distance between two points found at the specified indexes
    in the provided vector.

    @param points A vector containing Points
    @param p1 The index in the array for the first point
    @param p2 The index in the array for the second point
    @return The distance between the points
*/
long dist(Point* points, int p1, int p2) {
    return points[p1].distance(points[p2]);
}

/**
    Implemented from Kattis pseudo code
    @param points An array with points to be traversed
    @return A greedy tour through the points
*/
long* greedyTour(Point* points, int n) {
    long* tour = new long[n];
    bool* used = new bool[n];
    tour[0] = 0;
    used[0] = true;

    for (int i=1; i < n; i++) {
        long best = -1;
        for (int j=0; j < n; j++) {
            if (!used[j] && (best == -1 || dist(points, tour[i-1], j) < dist(points, tour[i-1], best)))
                best = j;
        }
        tour[i] = best;
        used[best] = true;
    }
    return tour;
}

int main() {

    // Number of points
    int N;
    cin >> N;

    // List of points
    Point* points = new Point[N];
    for (int i=0; i < N; i++)
        points[i] = readPoint();

    // Tour
    long* tour = greedyTour(points, N);

    // Print results
    print_result(tour, N);

    return 0;
}
