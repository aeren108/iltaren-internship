#pragma once

#include "vector.h"
#include "list.h"

template <class T>
struct GraphNode {
	GraphNode(const T& item, int index) : item(item), index(index) {}
	T item;
	int index;
};

template <class T, C>
class Graph {
	public:
		Graph();

		void add_node(GraphNode<T>& item);
		void remove_node(const GraphNode<T>& item);
		void add_edge(const GraphNode<T>& i1, const GraphNode<T>& i2);
		void remove_edge(const GraphNode<T>& i1, const GraphNode<T>& i2);

	private:
		Vector<List<GraphNode<T>, C>, C> edges;
		Vector<GraphNode<T>*, C> nodes;
};

template <class T, int C>
void Graph<T, C>::add_node(GraphNode<T>& item) {
	item.index = nodes.size();
	nodes.push_back(item);
	edges.push_back(List<GraphNode<T>*, C>());
}

template <class T, int C>
void Graph<T, C>::remove_node(const GraphNode<T>& item) {
	for (int i = 0; i < nodes.size()) {
		edges[i].remove(&nodes[item.index]);
	}

	edges.remove(item.index);
	nodes.remove(item.index);
}

void Graph<T, C>::remove_edge(GraphNode<T>& n1, GraphNode<T>& t2) {

}