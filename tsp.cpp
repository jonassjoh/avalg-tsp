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
    int index;
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
    Point(int index, double x, double y) {
      this->index = index;
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
    int to;
    int from;
    int savings;

    Edge(int to, int from, int savings) {
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
Point readPoint(int i) {
    double x, y;
    cin >> x >> y;
    Point a(i, x, y);
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
int dist(Point p1, Point p2) {
    return (int) (sqrt( pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)) + 0.5L);
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
            if (!used[j] && (best == -1 || dist(points[tour[i-1]], points[j]) < dist(points[tour[i-1]], points[best])))
              best = j;
        }
        tour[i] = best;
        used[best] = true;
    }
    return tour;
}

bool containsCycle(vector<vector<int>> &tour, Edge edge) {
    int prev = -1;
    int next = edge.from;
    int old_next = -1;

    while(next != old_next) {
        old_next = next;
        /* code */
        for(int i = 0; i < tour[next].size(); i++) {
            if(tour[next][i] != prev) {
                prev = next;
                next = tour[next][i];
                break;
            }

            if(next == edge.to) {
                //cycle found
                return true;
            }
        }
    }

    return false;
}

vector<vector<int>> clarke_wright(vector<Point> points, int n) {
    //srand(time(NULL));
    //int hub_index = rand() % n;
    Point hub = points[0];

    vector<Edge> edges;

    vector<vector<int>> tour;
    tour.reserve(n);

    //V_h = V - h
    points.erase(std::remove(points.begin(), points.end(), hub), points.end());

    for(int i = 0; i < points.size(); i++) {
        points[i].parent = &points[i];
        cout << "Point without hub : " << points[i] << endl;
        for(int j = i + 1; j < points.size(); j++) {
            int val = dist(hub, points[i]) + dist(hub, points[j]) - dist(points[j], points[i]);
            edges.push_back(Edge(points[i].index, points[j].index, val));
        }
    }

    //sort the list of Edges
    sort(edges.begin(), edges.end(), less_than_Edge());

    //Preprocessing done

    cout << "Hehehe" << endl;

    while(points.size() > 2) {
        Edge &e = edges[0];

        cout << "Never f****d up" << endl;


        if(!containsCycle(tour, e)) {

        }

    }

    return tour;
}

int main() {

    // Number of points
    int N;
    cin >> N;

    // List of points
    vector<Point> points;
    for (int i=0; i < N; i++)
    points.push_back(readPoint(i));
    // Tour
    int* tour = greedyTour(points, N);

    // Print results
    print_result(tour, N);

    vector<vector<int>> apa = clarke_wright(points, N);

    return 0;
}
