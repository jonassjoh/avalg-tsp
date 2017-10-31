#include <iostream>
#include <vector>
#include <math.h>
#include <limits>
#include <algorithm>
#include <map>

using namespace std;

/**
A class containing an x-coordinate and a y-coordinate to simulate
a point.
*/
class Point {
public:
    double x, y;
    Point* parent;
    int degree;

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
        this->degree = 0;
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
    int distance(Point p) {
        return (int) (sqrt( pow(this->x - p.x, 2) + pow(this->y - p.y, 2)) + 0.5L);
    }
};

bool operator!=(const Point& p1, const Point& p2) {
    return p1.x != p2.x && p1.y != p2.y;
}

bool operator<(const Point& p1, const Point& p2) {
    return p1.x < p2.x && p1.y < p2.y;
}

bool operator==(const Point& p1, const Point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

std::ostream & operator<<(std::ostream & Str, const Point& p) {
    Str << to_string(p.x) << ' ' << to_string(p.y);
    return Str;
}

struct Edge {
    Point to;
    Point from;
    int savings;

    Edge(Point to, Point from, int savings) {
        this->to = to;
        this->from = from;
        this->savings = savings;
    }
};

struct less_than_Edge
{
    inline bool operator() (const Edge& s1, const Edge& s2)
    {
        return (s1.savings > s2.savings);
    }
};

/**
Prints an array using a new line for every index

@param arr The array to be printed
@param size The length of the array
*/
void print_result(int* arr, int size) {
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
int dist(vector<Point> points, int p1, int p2) {
    return points[p1].distance(points[p2]);
}

/**
Implemented from Kattis pseudo code

@param points An array with points to be traversed
@return A greedy tour through the points
*/
int* greedyTour(vector<Point> points, int n) {
    int* tour = new int[n];
    bool* used = new bool[n]();
    tour[0] = 0;
    used[0] = true;

    for (int i=1; i < n; i++) {
        int best = -1;
        for (int j=0; j < n; j++) {
            if (!used[j] && (best == -1 || dist(points, tour[i-1], j) < dist(points, tour[i-1], best)))
            best = j;
        }
        tour[i] = best;
        used[best] = true;
    }
    return tour;
}

Point find(Point &x) {
    if (x.parent != x)
    x.parent = find(x.parent);
    return x.parent;
}

bool no_cycle(vector<Edge> edges, Edge e) {
    if (e.from.parent == e.to.parent) {
        return false;
    }
}

vector<Point> clarke_wright(vector<Point> points, int n) {
    //srand(time(NULL));
    //int hub_index = rand() % n;
    Point hub = points[0];

    vector<Edge> edges;

    vector<Edge> tour;

    //V_h = V - h
    points.erase(std::remove(points.begin(), points.end(), hub), points.end());

    for(int i = 0; i < points.size(); i++) {
        points[i].parent = points[i];
        cout << "Point without hub : " << points[i] << endl;
        for(int j = i + 1; j < points.size(); j++) {
            int val = dist(points, 0, i) + dist(points, 0, j) - dist(points, i, j);
            edges.push_back(Edge(&points[i], &points[j], val));
        }
    }

    //sort the list of Edges
    sort(edges.begin(), edges.end(), less_than_Edge());

    while(points.size() > 2) {
        for (int a=0; a < edges.size(); a++) {
            // Try vector pair (i, j) in Edges
            Edge e = edges[a];
            tour.push_back(e);
            e.from.degree++;
            e.to.degree++;
            if (no_cycle(edges, e) && e.from.degree <= 2 && e.to.degree <= 2) {
                if (e.from.degree == 2) {
                    //V_h = V - h
                    points.erase(std::remove(points.begin(), points.end(), e.from), points.end());
                }
            }
            else {
                e.from.degree--;
                e.to.degree--;
                tour.pop_back();
            }
        }
    }

    return points;
}

int main() {

    // Number of points
    int N;
    cin >> N;

    // List of points
    vector<Point> points;
    for (int i=0; i < N; i++)
    points.push_back(readPoint());
    // Tour
    int* tour = greedyTour(points, N);

    // Print results
    print_result(tour, N);

    vector<Point> apa = clarke_wright(points, N);
    return 0;
}
