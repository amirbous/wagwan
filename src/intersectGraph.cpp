#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include <ogdf/basic/ArrayBuffer.h>
#include <ogdf/basic/LayoutStatistics.h>
#include "../include/intersectGraph.hpp"




// Function to get the incident edges of a node u
std::vector<ogdf::edge> getIncidentEdges(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node &u) {
    std::vector<ogdf::edge> incident_edges;
    for (const auto & edge : G.edges) {
        if (edge->source() == u || edge->target() == u) {
            incident_edges.push_back(edge);
        }
    }
    return incident_edges;
}


std::pair<int, ogdf::edge> getODGFIntersect(const ogdf::Graph &G, const ogdf::GraphAttributes &GA) {
    ogdf::ArrayBuffer<int> crossings = ogdf::LayoutStatistics::numberOfCrossings(GA);
    int maxCrossings = std::numeric_limits<int>::min();
    ogdf::edge maxEdge = nullptr;

    int edgeIndex = 0;
    for (ogdf::edge e : G.edges) {
        if (crossings[e->index()] > maxCrossings) {
            maxCrossings = crossings[e->index()];
            maxEdge = e;
        }
        edgeIndex++;
    }
    return {maxCrossings, maxEdge};
}

int getODGFMax(const ogdf::Graph &G, const ogdf::GraphAttributes &GA) {
    ogdf::ArrayBuffer<int> crossings = ogdf::LayoutStatistics::numberOfCrossings(GA);
    int maxCrossings = std::numeric_limits<int>::min();
    ogdf::edge maxEdge = nullptr;

    int edgeIndex = 0;
    for (auto i = 0; i < crossings.size(); i++) {
        if (crossings[i] > maxCrossings) {
            maxCrossings = crossings[i];

        }
    }
    return maxCrossings;
}
// Function to check for shared nodes (edges can't intersect if they share a node)
bool isSharedNode(ogdf::node n1, ogdf::node n2, ogdf::node n3, ogdf::node n4) {
    return (n1 == n3 || n1 == n4 || n2 == n3 || n2 == n4);
}

// Function to calculate the orientation of three points
int orientation(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, ogdf::node c) {
    int val = (GA.y(c) - GA.y(a)) * (GA.x(b) - GA.x(a)) -
              (GA.y(b) - GA.y(a)) * (GA.x(c) - GA.x(a));
    return (val == 0) ? 0 : (val > 0 ? 1 : 2); // 0 -> collinear, 1 -> clockwise, 2 -> counterclockwise
}

// Function to check if a point c lies on segment (a, b) ((===> overload, calls function positionOnSegment))
bool nodeOnSegment(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, ogdf::node c) {
    std::pair<int, int> c_pos = {GA.x(c), GA.y(c)};
    return positionOnSegment(GA, a, b, c_pos);
}

// Function to check if a given (x, y) position lies on any edge of the graph
bool positionOnSegment(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, const std::pair<int, int>& pos) {
    int ax = (int)GA.x(a), ay = (int)GA.y(a);
    int bx = (int)GA.x(b), by = (int)GA.y(b);
    int cx = pos.first, cy = pos.second;

    if ((cx == ax && cy == ay) || (cx == bx && cy == by)) {
        return false;
    }

    int crossProduct = (cy - ay) * (bx - ax) - (cx - ax) * (by - ay);
    
    return (crossProduct == 0) && 
       (std::min(ax, bx) <= cx && cx <= std::max(ax, bx)) &&
       (std::min(ay, by) <= cy && cy <= std::max(ay, by));

}


// given node u, check if any of its incident edges intersect any other node of the graph 
bool nodeEdges_intersect_anyNode(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node & node) {
    std::vector<ogdf::edge> incident_edges = getIncidentEdges(G, GA, node);

    bool check = false;
    for (const auto & edge : incident_edges) {
        check |= edgeIntersects_anyNode(G, GA, edge);
    }
    return check;
}



// check if given edge new_edge intersects any nodes of the graph
bool edgeIntersects_anyNode(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::edge &new_edge) {
    bool check = false;
    for (const auto & node : G.nodes) {
        check |= nodeOnSegment(GA, new_edge->source(), new_edge->target(), node);
    }
    return check;
}



// check if given graph node new_source lies on any other edge of the graph
bool check_node_on_anyEdge(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node &new_source)
{
    bool check = false;

    for (auto edge: G.edges)
    {
        check |= nodeOnSegment(GA, edge->source(), edge->target(), new_source);
    }
    return check;
}

// check if given (x, y) position lies on any other edge of the graph
bool check_pos_on_anyEdge(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const std::pair<int, int>& pos)
{
    bool check = false;

    for (auto edge: G.edges)
    {
        check |= positionOnSegment(GA, edge->source(), edge->target(), pos);
    }
    return check;
}

bool edgesIntersect(const ogdf::GraphAttributes& GA, ogdf::edge& e1, ogdf::edge& e2) {
    // Retrieve the nodes for the edges
    ogdf::node a = e1->source();
    ogdf::node b = e1->target();
    ogdf::node c = e2->source();
    ogdf::node d = e2->target();

    // If edges share any node, they cannot intersect
    if (a == c || a == d || b == c || b == d) return false;

    // Orientation calculations
    auto orientation = [](const ogdf::GraphAttributes& GA, ogdf::node p, ogdf::node q, ogdf::node r) {
        float val = (GA.y(q) - GA.y(p)) * (GA.x(r) - GA.x(q)) - (GA.x(q) - GA.x(p)) * (GA.y(r) - GA.y(q));
        if (val == 0) return 0; // Collinear
        return (val > 0) ? 1 : 2; // Clockwise or Counterclockwise
    };

    // Check if a point lies on a segment
    auto nodeOnSegment = [](const ogdf::GraphAttributes& GA, ogdf::node p, ogdf::node q, ogdf::node r) {
        return (GA.x(r) <= std::max(GA.x(p), GA.x(q)) && GA.x(r) >= std::min(GA.x(p), GA.x(q)) &&
                GA.y(r) <= std::max(GA.y(p), GA.y(q)) && GA.y(r) >= std::min(GA.y(p), GA.y(q)));
    };

    int o1 = orientation(GA, a, b, c);
    int o2 = orientation(GA, a, b, d);
    int o3 = orientation(GA, c, d, a);
    int o4 = orientation(GA, c, d, b);

    // General case: edges intersect if orientations differ
    if (o1 != o2 && o3 != o4) return true;

    // Special cases: check for collinear points lying on segments
    if (o1 == 0 && nodeOnSegment(GA, a, b, c)) return true;
    if (o2 == 0 && nodeOnSegment(GA, a, b, d)) return true;
    if (o3 == 0 && nodeOnSegment(GA, c, d, a)) return true;
    if (o4 == 0 && nodeOnSegment(GA, c, d, b)) return true;

    // Collinear overlapping check
    if (o1 == 0 && o2 == 0 && o3 == 0 && o4 == 0) {
        if ((nodeOnSegment(GA, a, b, c) || nodeOnSegment(GA, a, b, d)) ||
            (nodeOnSegment(GA, c, d, a) || nodeOnSegment(GA, c, d, b))) {
            return true;
        }
    }

    return false; // No intersection
}