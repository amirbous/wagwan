
#include <iostream>
#include "../include/graph.hpp"
#include "../include/mathGeometry.hpp"
#include "../include/utils.hpp"
#include <bits/stdc++.h>

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

bool isSegmentBetween(Node a, Node b, Node c) {
    VectorGeometry v1 = VectorGeometry(a, b);
    VectorGeometry v2 = VectorGeometry(b, c);

    return wedge(v1, v2) == 0 && dot(v1, v2) > 0;
}

bool edgesIntersect(Graph g, Edge e1, Edge e2) {

        bool nonParallelIntersect = false;
        Node a = g.getNode(e1.getSource_id());
        Node b = g.getNode(e1.getTarget_id());
        
        Node c = g.getNode(e2.getSource_id());
        Node d = g.getNode(e2.getTarget_id());

        
        nonParallelIntersect = clockwiseOrientation(a,c,d) != clockwiseOrientation(b,c,d) && clockwiseOrientation(a,b,c) != clockwiseOrientation(a,b,d);

        // 3 cases is enough, to be verified but pure reason ;)
        return nonParallelIntersect || isSegmentBetween(a, b, c) // general case intersection 
            || isSegmentBetween(a, b, d) || isSegmentBetween(c, d, a) // Overlapping parallel segments
            || VectorGeometry(a, c).isNull() || VectorGeometry(b, d).isNull() // supperposed Nodes 1
            || VectorGeometry(a, d).isNull() || VectorGeometry(b, c).isNull(); // supperposed nodes supperposed Nodes 2

}

std::vector<std::pair<Edge, Edge>> findIntersections(Graph& graph) {
    struct Event {
        int x;
        bool isStart;
        Edge edge;

        bool operator<(const Event& other) const {
            return x < other.x || (x == other.x && isStart < other.isStart);
        }
    };

    // Prepare events
    std::vector<Event> events;
    for (auto edge : graph.getEdges()) {
        Node p1 = graph.getNode(edge.first.getSource_id());
        Node p2 = graph.getNode(edge.first.getTarget_id());

        if (p1.getX() > p2.getX()) std::swap(p1, p2);

        events.push_back({p1.getX(), true, edge.first});
        events.push_back({p2.getX(), false, edge.first});
    }

    std::sort(events.begin(), events.end());

    // Active set to hold edges as they are processed
    std::set<Edge> activeSet;
    std::vector<std::pair<Edge, Edge>> intersections;

    for (auto event : events) {
        Edge edge = event.edge;

        if (event.isStart) {
            // Add edge to active set and check for intersections
            for (Edge activeEdge : activeSet) {

                if (edgesIntersect(graph, edge, activeEdge)) {
                    intersections.emplace_back(edge, activeEdge);
                }
            }
            activeSet.insert(edge);
        } else {
            // Remove edge from active set
            activeSet.erase(edge);
        }
    }

    return intersections;
}