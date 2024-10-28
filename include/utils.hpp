#ifndef utils_HPP
#define utils_HPP

#include <iostream>
#include "../include/graph.hpp"

class Edge;
class Node;

// Function to check if 2 edges intersect
bool edgesIntersect (Graph g, Edge e1, Edge e2);

int sign(int x);

bool inter1(int a, int b, int c, int d);

int cross(Node a, Node b);
int cross(Node a, Node b, Node c);

bool check_inter(Node a, Node b, Node c, Node d);

#endif