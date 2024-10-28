
#include <iostream>
#include "../include/graph.hpp"
#include "../include/utils.hpp"

#define MAX(a, b) a ? a > b : b
#define MIN(a, b) a ? a < b : b

class Node;
class Edge;
class Graph; 
/**
 * Function edge Intersect
 *
 * Given a graph, and 2 edges on the graph, checks if the edges intersect
 *
 * @param Graph: current graph
 * @param first_edge: first edge from edge pair
 * @param second_edge: second edge from edge pair
 * @return true if edges intersect, false otherwise
 */

// TODO: the function should resolve on the grid not on the graph ( on points not on nodes )

int sign(int x) { return x >= 0 ? x ? 1 : 0 : -1; }

int cross(Node a, Node b) { return a.getX() * b.getY() - a.getY() * b.getX(); }
int cross(Node a, Node b, Node c) {
  return cross(b-a,c-a);
}

bool inter1(int a, int b, int c, int d) {
    if (a > b)
        std::swap(a, b);
    if (c > d)
        std::swap(c, d);
    return std::max(a, c) <= std::min(b, d);
}

bool check_inter(Node a, Node b, Node c, Node d) {
    if (cross(c,a,d) == 0 && cross(c,b, d) == 0)
        return inter1(a.getX(), b.getX(), c.getX(), d.getX()) && inter1(a.getY(), b.getY(), c.getY(), d.getY());
    return sign(cross(a,b, c)) != sign(cross(a,b, d)) &&
           sign(cross(c,d, a)) != sign(cross(c,d, b));
}

bool edgesIntersect (Graph g, Edge e1, Edge e2) {
  Node a = g.getNode(e1.getSource_id());
  Node b = g.getNode(e1.getTarget_id());
  Node c = g.getNode(e2.getSource_id());
  Node d = g.getNode(e2.getTarget_id());
  return check_inter(a,b,c,d);
}