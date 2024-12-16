#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/intersectGraph.hpp"



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

// Function to check if a point c lies on segment (a, b)
bool nodeOnSegment(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, ogdf::node c) {
    std::pair<int, int> c_pos = {GA.x(c), GA.y(c)};
    return positionOnSegment(GA, a, b, c_pos);
}
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

bool check_node_on_anyEdge(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node &new_source)
{
    bool check = false;

    for (auto edge: G.edges)
    {
        check |= nodeOnSegment(GA, edge->source(), edge->target(), new_source);
    }
    return check;
}
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
    // Get the nodes of the edges
    ogdf::node a = e1->source();
    ogdf::node b = e1->target();
    ogdf::node c = e2->source();
    ogdf::node d = e2->target();

    // If any of the edges share a node, return false immediately
    if (isSharedNode(a, b, c, d)) return false;

    // Calculate orientations for the pairs of points
    int o1 = orientation(GA, a, b, c);
    int o2 = orientation(GA, a, b, d);
    int o3 = orientation(GA, c, d, a);
    int o4 = orientation(GA, c, d, b);

    // If orientations are different, the edges intersect
    if (o1 != o2 && o3 != o4) return true;

    // Handle special cases where points are collinear but lie on the segment
    // Perform this check only if the edges are collinear
    if (o1 == 0) {
        if (nodeOnSegment(GA, a, b, c) || nodeOnSegment(GA, a, b, d)) return true;
    }
    if (o2 == 0) {
        if (nodeOnSegment(GA, a, b, d)) return true;
    }
    if (o3 == 0) {
        if (nodeOnSegment(GA, c, d, a)) return true;
    }
    if (o4 == 0) {
        if (nodeOnSegment(GA, c, d, b)) return true;
    }

    // Additional check for collinear segments that just touch at one endpoint
    // Ensure segments are touching at one endpoint or overlap slightly
    if (o1 == 0 && o2 == 0 && o3 == 0 && o4 == 0) {
        // Ensure segments are touching at one endpoint
        if ((nodeOnSegment(GA, a, b, c) && nodeOnSegment(GA, a, b, d)) || 
            (nodeOnSegment(GA, c, d, a) && nodeOnSegment(GA, c, d, b))) {
            return true;
        }
    }

    return false; // No intersection
}
