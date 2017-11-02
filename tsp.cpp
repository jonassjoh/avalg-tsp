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

bool containsCycle(vector<Point> &points, Edge edge) {
    return points[edge.from].parent == points[edge.to].parent;
}

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

vector<vector<int>> clarke_wright(vector<Point> points, int n) {
    //srand(time(NULL));
    //int hub_index = rand() % n;
    Point hub = points[0];
    vector<Edge> edges;

    bool* bools = new bool[n]();

    vector<vector<int>> tour(n);
    // tour.reserve(n);

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

        //edges.erase(edges.begin() + 0);
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

void print_tour(vector<vector<int>> &tour) {

    if (tour[0].size() == 0) {
        cout << 0 << endl;
        return;
    }

    int prev = 0;
    int next = tour[0][0];

    cout << prev << endl;
    while(next != 0) {
        /* code */
        for(int i = 0; i < tour[next].size(); i++) {
            if(tour[next][i] != prev) {
                cout << next << endl;
                if (i == 1) {
                    swap( tour[next][0], tour[next][1] );
                }
                prev = next;
                next = tour[next][0];
                break;
            }
        }
    }
}

int tour_length(vector<vector<int>> &tour, vector<Point> &points) {
    int l = 0;

    if (tour[0].size() == 0) {
        return l;
    }

    int prev = 0;
    int next = tour[0][0];

    while(next != 0) {
        /* code */
        for(int i = 0; i < tour[next].size(); i++) {
            if(tour[next][i] != prev) {
                l += points[next].distance(points[prev]);
                if (i == 1) {
                    swap( tour[next][0], tour[next][1] );
                }
                prev = next;
                next = tour[next][0];
                break;
            }
        }
    }
    l += points[next].distance(points[prev]);

    return l;
}

static bool IsOnSegment(double xi, double yi, double xj, double yj,
                        double xk, double yk) {
  return (xi <= xk || xj <= xk) && (xk <= xi || xk <= xj) &&
         (yi <= yk || yj <= yk) && (yk <= yi || yk <= yj);
}

static char ComputeDirection(double xi, double yi, double xj, double yj,
                             double xk, double yk) {
  double a = (xk - xi) * (yj - yi);
  double b = (xj - xi) * (yk - yi);
  return a < b ? -1 : a > b ? 1 : 0;
}

/** Do line segments (x1, y1)--(x2, y2) and (x3, y3)--(x4, y4) intersect? */
bool DoLineSegmentsIntersect(double x1, double y1, double x2, double y2,
                             double x3, double y3, double x4, double y4) {
  char d1 = ComputeDirection(x3, y3, x4, y4, x1, y1);
  char d2 = ComputeDirection(x3, y3, x4, y4, x2, y2);
  char d3 = ComputeDirection(x1, y1, x2, y2, x3, y3);
  char d4 = ComputeDirection(x1, y1, x2, y2, x4, y4);
  return (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
          ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) ||
         (d1 == 0 && IsOnSegment(x3, y3, x4, y4, x1, y1)) ||
         (d2 == 0 && IsOnSegment(x3, y3, x4, y4, x2, y2)) ||
         (d3 == 0 && IsOnSegment(x1, y1, x2, y2, x3, y3)) ||
         (d4 == 0 && IsOnSegment(x1, y1, x2, y2, x4, y4));
}

bool DoLineSegmentsIntersect(Point &p1, Point &p2, Point &p3, Point &p4) {
    return DoLineSegmentsIntersect(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
}

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

int get_path_length(vector<int> &path, vector<Point> &points) {
    int s = 0;
    if (path.size() == 1) return 0;
    for (int i=1; i < path.size(); i++) {
        s += points[path[i-1]].distance(points[path[i]]);
    }
    s += points[path[0]].distance(points[path[path.size()-1]]);
    return s;
}

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

void get_k2opt_path(vector<int> &path, int p1, int p2) {
    reverse(path.begin()+p1, path.begin()+p2+1);
}

vector<int> k2opt(vector<vector<int>> &tour, vector<Point> &points) {

    vector<int> path = get_path(tour);
    if (tour[0].size() == 0) return path;

    int path_length = get_path_length(path, points);

    while (true) {
        outer:;
        for (int p1=1; p1 < path.size(); p1++) {
            for (int p2=p1+1; p2 < path.size()-1; p2++) {

                Point &pivot_1 = points[path[p1-1]];    // 1
                Point &pivot_2 = points[path[p2]];      // 4
                Point &pivot_3 = points[path[p2+1]];    // 5
                Point &pivot_4 = points[path[p1]];      // 2

                int savings = get_savings(pivot_1, pivot_2, pivot_3, pivot_4);
                int primedistance = path_length - savings;

                if (primedistance < path_length) {
                    get_k2opt_path(path, p1, p2);
                    path_length = primedistance;
                    break;
                    //goto outer;
                }
            }
        }
        break;
    }

    return path;
    /*
    int tourLength = tour_length(tour, points);
    bool noChange = true;
    do {
        /*
            For Example:

            0 -> 1 -> 2     0    1 -> 2
            ^         |     ^  \      |
            |         v     |      \  v
            5 <- 4 <- 3     5 <- 4    3

            tour = [
                0 => [1, 5]   -->   0 => [__3__, 5] (pivot 1)
                1 => [2, 0]   -->   1 => [2, __4__]
                2 => [3, 1]
                3 => [4, 2]   -->   3 => [__0__, 2]
                4 => [5, 3]   -->   4 => [5, __1__] (pivot 2)
                5 => [0, 4]
            ]
        */ /*
        for (int p1=0; p1 < tour.size(); p1++) {
            for (int p2=p1+1; p2 < tour.size(); p2++) {
                if (p1 != p2 && p2 != tour[p1][0] && p2 != tour[p1][1]) {
                    // p1 and p2 are 2 different points that are not neighbours

                    int p1_next = tour[p1][0];  // 1
                    int p2_next = tour[p2][1];  // 3

                    int p1_next_old = tour[p1_next][1];
                    int p2_next_old = tour[p2_next][0];

                    tour[p1][0] = p2_next;      // 0 => [3, 5]
                    tour[p2][1] = p1_next;      // 4 => [5, 1]

                    tour[p1_next][1] = p2;      // 1 => [2, 4]
                    tour[p2_next][0] = p1;      // 3 => [0, 2]

                    /*
                        p1 used to go to p1_next, now goes to p2_next
                        p2 used to go to p2_next, now goes to p1_next
                    */ /*

                    int p1_old_distance = points[p1].distance(points[p1_next]);
                    int p2_old_distance = points[p2].distance(points[p2_next]);

                    int p1_new_distance = points[p1].distance(points[p2_next]);
                    int p2_new_distance = points[p2].distance(points[p1_next]);

                    int p1_distance_gain = p1_old_distance - p1_new_distance;
                    int p2_distance_gain = p2_old_distance - p2_new_distance;

                    int total_distance_gain = p1_distance_gain + p2_distance_gain;

                    //int primetour_length = tour_length(primetour, points);
                    int primetour_length = tourLength - total_distance_gain;
                    if (primetour_length < tourLength) {
                        tourLength = primetour_length;
                        noChange = false;

                        //p1 = tour.size();
                        break;
                        //goto outer;
                    } else {
                        // Revert path
                        tour[p1][0] = p1_next;
                        tour[p2][1] = p2_next;
                        tour[p1_next][1] = p1_next_old;
                        tour[p2_next][0] = p2_next_old;
                    }
                }
            }
        }
        noChange = true;
        outer:;
    } while(!noChange);
    return tour;
    */
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
