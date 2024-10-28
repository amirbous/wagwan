
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

bool clockwiseOrientation(Node a, Node b, Node c) {
    return (c.getY()-a.getY())*(b.getX()-a.getX()) > (b.getY()-a.getY())*(c.getX()-a.getX());

}
bool edgesIntersect(Graph g, Edge e1, Edge e2) {

        Node a = g.getNode(e1.getSource_id());
        Node b = g.getNode(e1.getTarget_id());
        
        Node c = g.getNode(e2.getSource_id());
        Node d = g.getNode(e2.getTarget_id());

        
        return clockwiseOrientation(a,c,d) != clockwiseOrientation(b,c,d) && clockwiseOrientation(a,b,c) != clockwiseOrientation(a,b,d);

}