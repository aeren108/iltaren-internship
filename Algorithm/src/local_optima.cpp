#include "local_optima.h"

double FirstOrderNeighbors::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
	int neighbours = 0;
	for (int i = 0; i < SIZE; ++i) {
		if (graph[node][i] && i != node && !visited[i]) neighbours++;
	}

	return 1.0 - 1.0 / neighbours;
}

int SecondOrderNeighbors::neighbour_count(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
	int neighbours = 0;
	for (int i = 0; i < SIZE; ++i ) {
		if (graph[node][i] && i != node && !visited[i]) neighbours++;
	}

	return neighbours;
}

double SecondOrderNeighbors::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    bool old = visited[node];
    visited[node] = true;

    int max = 0, maxi = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (graph[node][i] && !visited[i]) {
            int cur = neighbour_count(i, graph, visited);
            if (cur > max) {
                max = cur;
                maxi = i;
            }
        }
    }

    visited[node] = old;
    return 1.0 - 1.0 / max;

    /*List<int, SIZE> queue;
    queue.push_back(node);
    int dists[SIZE];
    int max = 0;

    for (int i = 0; i < SIZE; ++i) { visited[i] = false; dists[i] = 0; };

    while (!queue.empty()) {
        int cur = queue[0];
        queue.remove_at(0);

        if (dists[cur] == 1) {
            int c = neighbour_count(cur, graph, visited);
            if (c > max) max = c;
        }
        else if (dists[cur] > 1) break;

        if (!visited[cur]) {
            for (int i = 0; i < SIZE; ++i) {
                if (graph[cur][i] && !visited[i]) {
                    dists[i] = dists[cur] + 1;
                    queue.push_back(i);
                }
            }

            visited[cur] = true;
        }
    }

    return max;*/
}

int ThirdOrderNeighbors::neighbour_count(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    int neighbours = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (i != node && graph[node][i] && visited[i]) neighbours++;
    }

    return neighbours;
}

int ThirdOrderNeighbors::neighbour_count2(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    bool old = visited[node];
    visited[node] = true;

    int max = 0, maxi = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (graph[node][i]) {
            int cur = neighbour_count(i, graph, visited);
            if (cur > max) {
                max = cur;
                maxi = i;
            }
        }
    }

    visited[node] = old;
    return max;
}

double ThirdOrderNeighbors::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    bool old = visited[node];
    visited[node] = true;

    int max = 0, maxi = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (graph[node][i] && !visited[i]) {
            int cur = neighbour_count2(i, graph, visited);
            if (cur > max) {
                max = cur;
                maxi = i;
            }
        }
    }
    visited[node] = old;

    return 1.0 - 1.0 / max;
}

double ClosenessCentrality::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    int* dists = compute_distances(node, graph); int sum = 0;
    for (int i = 0; i < SIZE; ++i) sum += dists[i];
    return 1.0 / sum;
}

//dijskta's algorithm for shortest path becomes breadth first search for unweighted graphs
int* ClosenessCentrality::compute_distances(int start, Vector<Vector<int, SIZE>, SIZE>& graph) {
    List<int, SIZE> queue;
    queue.push_back(start);
    bool visited[SIZE];
    static int dists[SIZE];

    for (int i = 0; i < SIZE; ++i) { visited[i] = false; dists[i] = 0; };

    while (!queue.empty()) {
        int cur = queue[0];
        queue.remove_at(0);

        if (!visited[cur]) {
            for (int i = 0; i < SIZE; ++i) {
                if (graph[cur][i] && !visited[i]) {
                    dists[i] = dists[cur] + 1;
                    queue.push_back(i);
                }
            }

            visited[cur] = true;
        }
    }
    return dists;
}

//Takes too much time ?
double BetweennessCentrality::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    int total = 0, containing = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = i; j < SIZE; ++j) {
            if (i == j) continue;

            int s = path_contains_node(node, i, j, graph);
            if (s == 1) containing += 1;
            if (s != -1) total += 1;
        }
    }
    return containing / (double) total;
}

int BetweennessCentrality::path_contains_node(int node, int start, int end, Vector<Vector<int, SIZE>, SIZE>& graph) {
    List<int, SIZE> queue;
    queue.push_back(start);
    bool visited[SIZE];
    int prevs[SIZE];

    for (int i = 0; i < SIZE; ++i) { visited[i] = false; prevs[i] = -1; };

    while (!queue.empty()) {
        int cur = queue[0];
        queue.remove_at(0);

        if (cur == end) break;

        if (!visited[cur]) {
            for (int i = 0; i < SIZE; ++i) {
                if (graph[cur][i] && !visited[i]) {
                    prevs[i] = cur;
                    queue.push_back(i);
                }
            }

            visited[cur] = true;
        }
    }

    if (prevs[end] != -1) {
        int cur = end;
        while (cur != start) {
            if (cur == node) {
                return 1;
            }

            cur = prevs[cur];
        }
        return 0;
    } else return -1;
}

double DistanceToPrevious::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    int sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (graph[node][i] && visited[i]) {
            sum += compute_distance(node, i, graph);
        }
    }
    return 1.0 / sum;
}

int DistanceToPrevious::compute_distance(int start, int end, Vector<Vector<int, SIZE>, SIZE>& graph) {
    List<int, SIZE> queue;
    queue.push_back(start);
    bool visited[SIZE];
    int dists[SIZE];

    for (int i = 0; i < SIZE; ++i) { visited[i] = false; dists[i] = 1000; };
    dists[start] = 0;

    while (!queue.empty()) {
        int cur = queue[0];
        queue.remove_at(0);

        if (cur == end) break;
        
        if (!visited[cur]) {
            for (int i = 0; i < SIZE; ++i) {
                if (graph[cur][i] && !visited[i]) {
                    dists[i] = dists[cur] + 1;
                    queue.push_back(i);
                }
            }

            visited[cur] = true;
        }
    }
    
    return dists[end];
}


double DistanceToStart::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited_[SIZE]) {
    List<int, SIZE> queue;
    queue.push_back(start);
    bool visited[SIZE];
    int dists[SIZE];

    for (int i = 0; i < SIZE; ++i) { visited[i] = false; dists[i] = 1000; };
    dists[node] = 0;

    while (!queue.empty()) {
        int cur = queue[0];
        queue.remove_at(0);

        if (cur == start) break;

        if (!visited[cur]) {
            for (int i = 0; i < SIZE; ++i) {
                if (graph[cur][i] && !visited[i]) {
                    dists[i] = dists[cur] + 1;
                    queue.push_back(i);
                }
            }

            visited[cur] = true;
        }
    }

    return 1.0 / dists[start];
}


LinearCombination::LinearCombination(double weights[4], int start) {
    for (int i = 0; i < 4; ++i) this->weights[i] = weights[i];
    h4.start = start;
}

double LinearCombination::score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) {
    double sum = 0;
    for (int i = 0; i < 4; ++i) 
        sum += weights[i] * hs[i]->score(node, graph, visited);
    return sum;
}