#ifndef utils_HPP
#define utils_HPP

#include <iostream>
#include "../include/graph.hpp"

class Edge;
class Node;

// Function to check if 2 edges intersect
bool onSegment(Node p, Node q, Node r);
int orientation(Node p, Node q, Node r);
bool edgesIntersect (Graph graph, Edge first_edge, Edge second_edge);


#endif