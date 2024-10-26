
#include <iostream>
#include "../include/graph.hpp"
#include "../include/utils.hpp"

#define MAX(a, b) a ? a > b : b
#define MIN(a, b) a ? a < b : b
class Node;
class Edge;
class Graph; 
/**
 * Function edge Interset
 *
 * Given a graph, and 2 edges on the graph, checks if the edges intersect
 *
 * @param Graph: current graph
 * @param first_edge: first edge from edge pair
 * @param second_edge: second edge from edge pair
 * @return true if edges intersect, false otherwise
 */

// TODO: the function should resolve on the grid not on the graph ( on points not on nodes )

// Reference: Introduction to Algorithms 3rd Edition by Clifford Stein, Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest
//            http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf 
bool onSegment(Node p, Node q, Node r) 
{ 
    if (q.getX() <= MAX(p.getX(), r.getX()) && q.getX() >= MIN(p.getX(), r.getX()) && 
        q.getY() <= MAX(p.getY(), r.getY()) && q.getY() >= MIN(p.getY(), r.getY())) 
       return true; 
  
    return false; 
} 

int orientation(Node p, Node q, Node r) 
{ 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    int val = (q.getY() - p.getY()) * (r.getX() - q.getX()) - 
              (q.getX() - p.getX()) * (r.getY() - q.getY()); 
  
    if (val == 0) return 0;  // collinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
}

bool edgesIntersect (Graph graph, Edge first_edge, Edge second_edge) {

    int o1, o2, o3, o4;
    Node p1 = graph.getNode(first_edge.getSource_id()), 
            p2 = graph.getNode(first_edge.getTarget_id()), 
            q1 = graph.getNode(second_edge.getSource_id()), 
            q2 = graph.getNode(second_edge.getTarget_id());

    o1 = orientation(p1, q1, p2); 
    o2 = orientation(p1, q1, q2); 
    o3 = orientation(p2, q2, p1); 
    o4 = orientation(p2, q2, q1); 

    // General case
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are collinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are collinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are collinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 

}

