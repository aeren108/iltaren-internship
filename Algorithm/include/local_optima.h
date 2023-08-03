#include <iostream>
#include "vector.h"
#include "list.h"

#define SIZE 81

class LocalOptima {
public:

	virtual double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]) = 0;
};

class FirstOrderNeighbors : public LocalOptima {
public:
	
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
};

class SecondOrderNeighbors : public LocalOptima {
public:
	
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
private:
	int neighbour_count(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
};

class ThirdOrderNeighbors : public LocalOptima {
public:
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
private:
	int neighbour_count(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
	int neighbour_count2(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
};

class ClosenessCentrality : public LocalOptima {
public:
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
private:
	int* compute_distances(int start, Vector<Vector<int, SIZE>, SIZE>& graph);
};

class DistanceToPrevious : public LocalOptima {
public:
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
private:
	int compute_distance(int start, int end, Vector<Vector<int, SIZE>, SIZE>& graph);
};

class DistanceToStart : public LocalOptima {
public:
	DistanceToStart(int start = 25) : start(start) {}
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
	
	int start;
};

class BetweennessCentrality : public LocalOptima {
public:
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
private:
	int compute_betweenness(int start, Vector<Vector<int, SIZE>, SIZE>& graph);
	int path_contains_node(int node, int start, int end, Vector<Vector<int, SIZE>, SIZE>& graph);
};

class LinearCombination : public LocalOptima {
public:
	LinearCombination(double weights[4], int start);
	double score(int node, Vector<Vector<int, SIZE>, SIZE>& graph, bool visited[SIZE]);
	
	double weights[4];
private:
	FirstOrderNeighbors h0;
	SecondOrderNeighbors h1;
	ThirdOrderNeighbors h2;
	ClosenessCentrality h3;
	DistanceToStart h4;
	LocalOptima* hs[4] = { &h0, &h2, &h3, &h4 };
};