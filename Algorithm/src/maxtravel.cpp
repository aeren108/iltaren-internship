#include <iostream>
#include <string>

#include "vector.h"
#include "list.h"
#include "local_optima.h"

#define SIZE 81
#define PRINT_CITIES false

Vector<Vector<int, SIZE>, SIZE> cities;
LocalOptima* funcs[4];

void init_cities() {
    FILE *f = fopen("C:/Users/ahmet.colak/source/repos/iltaren-staj/file/ilmesafe.csv", "r");
    char buff[1024]; int i = 0;
    while (fgets(buff, 1024, f) != NULL) {
        if (i < 2 || i > SIZE + 1) { ++i; continue; }
        char* split = strtok(buff, ";");
        cities.push_back(Vector<int, SIZE>());
        
        split = strtok(NULL, ";"); split = strtok(NULL, ";");

        while (split != NULL) {
            cities[cities.size() - 1].push_back(std::stoi(split));

            split = strtok(NULL, ";");
        }

        ++i;
    }
    
}

int find_max_travel(int start, int prev, bool visited[SIZE], Vector<Vector<int, SIZE>, SIZE>& cities, LocalOptima& lo) {
    //filter unavailable paths
    if (prev != -1) {
        for (int i = 0; i < SIZE; ++i) cities[prev][i] = 0;
        for (int i = 0; i < SIZE; ++i) cities[i][prev] = 0;
    }
    
    visited[start] = true;
    double max = 0, maxi = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (cities[start][i]) {
            double cur = lo.score(i, cities, visited);
            if (cur > max) { max = cur; maxi = i; }
        }
    }

    if (maxi != -1) {
        if (PRINT_CITIES)
            std::cout << "Visited city: " << maxi + 1 << " Distance from " << start + 1 << " :" << cities[start][maxi] << std::endl;
        return 1 + find_max_travel(maxi, start, visited, cities, lo);
    }
    else return 1;
}

int edge_count(Vector<Vector<int, SIZE>, SIZE>& graph) {
    int edges = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (graph[i][j]) edges++;
        }
    }
    return edges;
}

int find_max_travel_probabilistic(int start, int prev, bool visited[SIZE], Vector<Vector<int, SIZE>, SIZE>& graph) {
    //filter unavailable paths
    LocalOptima* curfunc = funcs[2];
    #define EDGE_THRESHOLD 45
    
    if (prev != -1) {
        for (int i = 0; i < SIZE; ++i) graph[prev][i] = 0;
        for (int i = 0; i < SIZE; ++i) graph[i][prev] = 0;
    }


    double probability = edge_count(graph) * 100.0 / edge_count(cities);
    if (funcs[3]->score(start, graph, visited) > 0.3) curfunc = funcs[3]; else curfunc = funcs[2];
    if (edge_count(graph) > EDGE_THRESHOLD) curfunc = funcs[3];

    visited[start] = true;
    double max = 0, maxi = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (graph[start][i]) {

            double cur = curfunc->score(i, graph, visited);
            if (cur > max) { max = cur; maxi = i; }
        }
    }

    if (maxi != -1) {
        if (PRINT_CITIES)
            std::cout << "Visited city: " << maxi + 1 << " Distance from " << start + 1 << " :" << graph[start][maxi] << std::endl;
        return 1 + find_max_travel_probabilistic(maxi, start, visited, graph);
    }
    else return 1;
}

int qlearning_optimize_weights(int start) {
    Vector<Vector<int, SIZE>, SIZE> graph = cities;
    int num_states = 4, num_actions = 2, state = 0;
    double qtable[4][2], ntable[4][2];
    bool visited[SIZE];
    double weights[4];

    #define ITERATIONS 5000
    #define LEARNING_RATE 0.01
    #define EPSILON 10

    for (int i = 0; i < SIZE; ++i) visited[i] = false;
    for (int i = 0; i < num_states; ++i) {
        weights[i] = std::rand() % 100 / 100.0;
        for (int j = 0; j < num_actions; ++j) {
            qtable[i][j] = std::rand() % 100 / 100.0 ;
            ntable[i][j] = 0;
        }
    }
    LinearCombination lcc(weights, start);
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH RANDOM WEIGHTS [" << weights[0] << ", " << weights[1] << ", " << weights[2] << ", " << weights[3] << "]: " << find_max_travel(start, -1, visited, graph, lcc) << std::endl;

    for (int i = 0; i < ITERATIONS; ++i) {
        state = i % num_states;

        int action = qtable[state][0] > qtable[state][1] ? 0 : 1;
        if (std::rand() % 100 < EPSILON) action = qtable[state][0] <= qtable[state][1] ? 0 : 1;

        graph = cities;
        for (int i = 0; i < SIZE; ++i) visited[i] = false;
        LinearCombination lc(weights, start);
        double reward = find_max_travel(start, -1, visited, graph, lc);

        if (action) weights[state] -= LEARNING_RATE * (SIZE - reward) / (double) SIZE; else weights[state] += LEARNING_RATE * reward / (double) SIZE;

        ntable[state][action] += 1;
        qtable[state][action] = qtable[state][action] + (reward - qtable[state][action]) * 0.1;
    }

    graph = cities;
    for (int i = 0; i < SIZE; ++i) visited[i] = false;
    LinearCombination lc(weights, start);
    int num = find_max_travel(start, -1, visited, graph, lc);

    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH WEIGHTS [" << weights[0] << ", " << weights[1] << ", " << weights[2] << ", " << weights[3] << "]: " << num << std::endl;

    return 0;
}

void init_graph(int distance, int tolerance, Vector<Vector<int, SIZE>, SIZE>& graph) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (graph[i][j] > distance + tolerance || graph[i][j] < distance - tolerance)
                graph[i][j] = 0;
        }
    }
}

void find_optimum_xy(LocalOptima& lo) {
    Vector<Vector<int, SIZE>, SIZE> graph = cities;
    int distance = 1000, tolerance = 100;
    bool visited[SIZE] = { false };
    int max = 0, maxdist = 0, maxtolerance = 0, globalmax = 0;

    for (; distance >= 100; distance -= 3) {
        graph = cities;
        init_graph(distance, tolerance, graph);
        for (int i = 0; i < SIZE; ++i) visited[i] = false;
        int cur = find_max_travel(5, -1, visited, graph, lo);
            
        if (cur > globalmax - 2) { maxdist = distance; max = cur; }
        if (cur > globalmax) { globalmax = cur; }
    }

    for (; tolerance > 10; tolerance -= 2) {
        graph = cities;
        init_graph(maxdist, tolerance, graph);
        for (int i = 0; i < SIZE; ++i) visited[i] = false;
        int cur = find_max_travel(5, -1, visited, graph, lo);
        
        if (cur > globalmax - 2) { maxtolerance = tolerance; max = cur; }
        if (cur > globalmax) { globalmax = cur; }
    }

    std::cout << "OPTIMUM DISTANCE AND TOLERANCE: " << maxdist << ", " << maxtolerance << " WITH CITY COUNT:" << max << std::endl;
}

void find_optimum_xy2(LocalOptima& lo) {
    Vector<Vector<int, SIZE>, SIZE> graph = cities;
    int distance = 1000, tolerance = 100;
    bool visited[SIZE] = { false };
    double max = 0;
    int maxdist = 0, maxtolerance = 0, globalmax = 0;

    for (; distance >= 100; distance -= 3) {
        for (; tolerance > 10; tolerance -= 2) {
            graph = cities;
            init_graph(distance, tolerance, graph);
            //for (int i = 0; i < SIZE; ++i) visited[i] = false;
            //int cur = find_max_travel(5, -1, visited, graph, lo);

            double nt = tolerance / 100.0, nd = (distance - 100) / 100.0;
            int edges = edge_count(graph); for (int i = 0; i < SIZE; ++i) visited[i] = false;
            double score = (double)find_max_travel(5, -1, visited, graph, lo) / (nt + nd);
            if (score > max) { max = score; maxdist = distance; maxtolerance = tolerance; }
        }
    }

    
    init_graph(maxdist, maxtolerance, graph);
    std::cout << "OPTIMUM DISTANCE AND TOLERANCE: " << maxdist << ", " << maxtolerance << " WITH CITY COUNT:" << find_max_travel(5, -1, visited, graph, lo) << std::endl;
}

int main() {
    bool visited[SIZE]; //TODO: Read distance and tolerance values from standart input
    int distance = 250; int tolerance = 50; int start = 5;

    //double weights[4] = { -0.1, 0.25, 0.4, 0.45 }; //for ankara only
    double weights[4] = { -0.1, 0.2, 0.5, 0.1 };

    FirstOrderNeighbors fon; SecondOrderNeighbors son; ThirdOrderNeighbors ton; ClosenessCentrality cc;
    funcs[0] = &fon; funcs[1] = &son; funcs[2] = &ton; funcs[3] = &cc;

    init_cities();
    find_optimum_xy(LinearCombination(weights, start));


    init_graph(distance, tolerance, cities);

    for (int i = 0; i < SIZE; ++i) visited[i] = false; Vector<Vector<int, SIZE>, SIZE> graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH FIRST ORDER NEIGHBORS: " << find_max_travel(start, -1, visited, graph, FirstOrderNeighbors()) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH SECOND ORDER NEIGHBORS: " << find_max_travel(start, -1, visited, graph, SecondOrderNeighbors()) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH THIRD ORDER NEIGHBORS: " << find_max_travel(start, -1, visited, graph, ThirdOrderNeighbors()) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH CLOSENESS CENTRALITY: " << find_max_travel(start, -1, visited, graph, ClosenessCentrality()) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH BETWEENNESS CENTRALITY: " << find_max_travel(start, -1, visited, graph, BetweennessCentrality()) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH RANDOM HEURISTIC AT EACH STEP: " << find_max_travel_probabilistic(start, -1, visited, graph) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH LINEAR COMBINATION: " << find_max_travel(start, -1, visited, graph, LinearCombination(weights, start)) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH DISTANCE TO PREVIOUS: " << find_max_travel(start, -1, visited, graph, DistanceToPrevious()) << std::endl;
    for (int i = 0; i < SIZE; ++i) visited[i] = false; graph = cities;
    std::cout << "MAX CITY COUNT FROM " << start + 1 << " WITH DISTANCE TO START: " << find_max_travel(start, -1, visited, graph, DistanceToStart(start)) << std::endl;

    qlearning_optimize_weights(start);

    return 1;
}