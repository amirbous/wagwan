#ifndef utils_HPP
#define utils_HPP

#include <iostream>
#include "../include/graph.hpp"

class Edge;
class Node;


bool clockwiseOrientation(Node a, Node b, Node c);
bool edgesIntersect(Graph g, Edge e1, Edge e2);
#endif