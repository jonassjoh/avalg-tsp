#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

class Point;

/**
    A class containing an x-coordinate and a y-coordinate to simulate
    a point.
*/
class Point {
public:
    double x, y;
    int rank;
    Point* parent;

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

class Edge {
public:
    int a, b;
    long distance;
    Edge(int a, int b, long distance) {
        this->a = a;
        this->b = b;
        this->distance = distance;
    }

    bool operator > (const Edge &other) {
        return this->distance > other.distance;
    }
    bool operator < (const Edge &other) {
        return this->distance < other.distance;
    }
};

class Graph {
public:
    vector<Point> points;
    vector<Edge> edges;
    Graph(Point* points, int n) {
        for (int i=0; i < n; i++) {

            // Add point to graph
            this->points.push_back(points[i]);

            // Add all edges from point to other points
            for (int j=i+1; j < n; j++) {
                Edge edge(i, j, points[i].distance(points[j]));
                edges.push_back(edge);
            }
        }
        sort(edges.begin(), edges.end());
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
long dist(Point* points, int p1, int p2) {
    return points[p1].distance(points[p2]);
}

/**
    Implemented from Kattis pseudo code

    @param points An array with points to be traversed
    @return A greedy tour through the points
*/
int* greedyTour(Point* points, int n) {
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

void makeSet(vector<Point> points, int i) {
    points[i].parent = &points[i];
    points[i].rank = 0;
}

Point* find(Point x) {
    if (x.parent != &x)
        x.parent = find(*(x.parent));
    return x.parent;
}

void kruskal_union(Point x, Point y) {
    Point xRoot = *find(x);
    Point yRoot = *find(y);

    if (&xRoot == &yRoot) return;

    if (xRoot.rank < yRoot.rank)
        xRoot.parent = &yRoot;
    else if (xRoot.rank > yRoot.rank)
        yRoot.parent = &xRoot;
    else {
        yRoot.parent = &xRoot;
        xRoot.rank = xRoot.rank + 1;
    }
}

void kruskal(Graph graph) {
    vector<Edge> A;
    for (int i=0; i < graph.points.size(); i++) {
        makeSet(graph.points, i);
    }
    for (int i=0; i < graph.edges.size(); i++) {
        Edge edge = graph.edges[i];
        if (find(graph.points[edge.a]) != find(graph.points[edge.b])) {
            A.push_back(edge);
            kruskal_union(graph.points[edge.a], graph.points[edge.b]);
        }
    }
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
    int* tour = greedyTour(points, N);

    Graph graph(points, N);

    kruskal(graph);

    // Print results
    print_result(tour, N);

    return 0;
}
