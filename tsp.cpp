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

    int parent;
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
        return (s1.savings < s2.savings);
    }
};

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

/**
Checks if adding an edge will produce a cycle. It does this by checking if
the parents of the two nodes connected by the edge are the same.
@param points A vector containing the Points in the Edge at the indexes
@param edge The edge that is added
@return true if adding the edge will produce a cycle, false otherwise.
*/
bool containsCycle(vector<Point> &points, Edge edge) {
    return points[edge.from].parent == points[edge.to].parent;
}

/**
Provided an edge, this function will mutate the Points connected by the edge
such that all points in turn connected by edges will share the same parent.
@param points A vector containing all Points present in the tour.
@param tour A vector och vectors containing all the neighbours of each point
@param edge The edge to match parents with
*/
void syncParents(vector<Point> &points, vector<vector<int>> &tour, Edge edge) {
    int parent = points[edge.to].parent;
    int prev = -1;
    int next = edge.from;
    while(points[next].parent != parent) {
        points[next].parent = parent;
        for(int i = 0; i < tour[next].size(); i++) {
            if(tour[next][i] != prev) {
                prev = next;
                next = tour[next][i];
                break;
            }
        }
    }
}

/**
TSP approximation using Clarke Wright
@param points All points in the graph
@param n The number of elements in points
*/
vector<vector<int>> clarke_wright(vector<Point> points, int n) {
    Point hub = points[0];
    vector<Edge> edges;

    bool* bools = new bool[n]();

    vector<vector<int>> tour(n);

    //V_h = V - h
    points.erase(std::remove(points.begin(), points.end(), hub), points.end());

    for(int i = 0; i < points.size(); i++) {
        points[i].parent = i;
        for(int j = i + 1; j < points.size(); j++) {
            int val = dist(hub, points[i]) + dist(hub, points[j]) - dist(points[j], points[i]);
            edges.push_back(Edge(points[i].index, points[j].index, val));
        }
    }

    //sort the list of Edges
    sort(edges.begin(), edges.end(), less_than_Edge());

    //Preprocessing done

    int bool_counter = n - 1;

    while(bool_counter > 2) {
        Edge e = edges[ edges.size()-1 ];

        if(!containsCycle(points, e) && tour[e.from].size() <= 1 && tour[e.to].size() <= 1) {

            syncParents(points, tour, e);

            tour[e.from].push_back(e.to);
            tour[e.to].push_back(e.from);

            if(tour[e.from].size() == 2) {
                bools[e.from] = true;
                bool_counter --;
            }
            if(tour[e.to].size() == 2) {
                bools[e.to] = true;
                bool_counter --;
            }
        }

        edges.pop_back();
    }

    for(int i = 1; i < n; i++) {
        if(!bools[i]) {
            tour[i].push_back(0);
            tour[0].push_back(i);
        }
    }

    return tour;
}

/**
Converts the neighbour list tour into a single vector.
For example:
tour = [
    [1 5]
    [2 0]
    [3 1]
    [4 2]
    [5 3]
    [0 4]
]
will return [0 1 2 3 4 5]

@param tour The multidimensional tour to be converted
@return A single one-dimensional vector representing the tour
*/
vector<int> get_path(vector<vector<int>> &tour) {
    vector<int> res;

    if (tour[0].size() == 0) {
        res.push_back(0);
        return res;
    }

    int prev = 0;
    int next = tour[0][0];

    while(next != 0) {
        for(int i = 0; i < tour[next].size(); i++) {
            if(tour[next][i] != prev) {
                res.push_back(prev);
                prev = next;
                next = tour[next][i];
                break;
            }
        }
    }
    res.push_back(prev);

    return res;
}

/**
Calculates the length of a path
@param path The path to calculate the distance for
@param points An array containing the position of all points in the path
@return The length of the entire path
*/
int get_path_length(vector<int> &path, vector<Point> &points) {
    int s = 0;
    if (path.size() == 1) return 0;
    for (int i=1; i < path.size(); i++) {
        s += points[path[i-1]].distance(points[path[i]]);
    }
    s += points[path[0]].distance(points[path[path.size()-1]]);
    return s;
}

/**
Calculates the gain from swapping two edges by comparing the distance before
and after swapping them around.
@param p1 The first pivot point
@param p2 The new point that p1 will connect to that p3 connected to before
@param p3 The second pivot point
@param p4 The new point that p3 will connect to that p1 connected to before
@return How much is gained by swapping the edges
*/
int get_savings(Point &p1, Point &p2, Point &p3, Point &p4) {
    int p1_old_distance = p1.distance(p4);
    int p3_old_distance = p3.distance(p2);

    int p1_new_distance = p1.distance(p2);
    int p3_new_distance = p3.distance(p4);

    int p1_distance_gain = p1_old_distance - p1_new_distance;
    int p3_distance_gain = p3_old_distance - p3_new_distance;

    int total_distance_gain = p1_distance_gain + p3_distance_gain;

    return total_distance_gain;
}

/**
Used in k2opt
Reverses the path from index p1 (included) to p2 (included).
Example:
    path = [0 1 2 3 4 5], p1=2, p2=4
    produces [0 1 4 3 2 5]

@param path The path to mutate
@param p1 The starting index (incuded) of the segment to reverse
@param p2 The ending index (included) of the segment to reverse
*/
void get_k2opt_path(vector<int> &path, int p1, int p2) {
    reverse(path.begin()+p1, path.begin()+p2+1);
}

/**
TSP improvement algorithm
@param tour The tour to improve
@param points A vector containing all the points in the tour
@return a one dimensional vector representing the improved tour
*/
vector<int> k2opt(vector<vector<int>> &tour, vector<Point> &points) {

    vector<int> path = get_path(tour);
    if (tour[0].size() == 0) return path;

    int best_savings = 0;
    do {
        best_savings = 0;
        for (int p1=1; p1 < path.size(); p1++) {
            for (int p2=p1+1; p2 < path.size()-1; p2++) {

                Point &pivot_1 = points[path[p1-1]];    // 1
                Point &pivot_2 = points[path[p2]];      // 4
                Point &pivot_3 = points[path[p2+1]];    // 5
                Point &pivot_4 = points[path[p1]];      // 2

                int savings = get_savings(pivot_1, pivot_2, pivot_3, pivot_4);

                if(savings > 0) {
                    get_k2opt_path(path, p1, p2);
                    best_savings = savings;
                }
            }
        }
    } while (best_savings != 0);

    return path;
}

int main() {

    // Number of points
    int N;
    cin >> N;

    // List of points
    vector<Point> points;
    for (int i=0; i < N; i++)
        points.push_back(readPoint(i));

    vector<vector<int>> tour = clarke_wright(points, N);

    vector<int> path = k2opt(tour, points);

    for (int i=0; i < path.size(); i++) {
        cout << path[i] << endl;
    }

    return 0;
}
