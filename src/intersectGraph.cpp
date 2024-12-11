#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/intersectGraph.hpp"

bool edgesIntersect(const ogdf::GraphAttributes& GA, ogdf::edge& e1, ogdf::edge& e2) {
    // Lambda to check for shared nodes (edges can't intersect if they share a node)
    auto isSharedNode = [](ogdf::node n1, ogdf::node n2, ogdf::node n3, ogdf::node n4) {
        return (n1 == n3 || n1 == n4 || n2 == n3 || n2 == n4);
    };

    // Lambda to calculate the orientation of three points
    auto orientation = [GA](ogdf::node a, ogdf::node b, ogdf::node c) {
        int val = (GA.y(c) - GA.y(a)) * (GA.x(b) - GA.x(a)) -
                  (GA.y(b) - GA.y(a)) * (GA.x(c) - GA.x(a));
        return (val == 0) ? 0 : (val > 0 ? 1 : 2); // 0 -> collinear, 1 -> clockwise, 2 -> counterclockwise
    };

    // Lambda to check if a point c lies on segment (a, b)
    auto onSegment = [GA](ogdf::node a, ogdf::node b, ogdf::node c) {
        return (std::min(GA.x(a), GA.x(b)) <= GA.x(c) && GA.x(c) <= std::max(GA.x(a), GA.x(b))) &&
               (std::min(GA.y(a), GA.y(b)) <= GA.y(c) && GA.y(c) <= std::max(GA.y(a), GA.y(b)));
    };

    // Get the nodes of the edges
    ogdf::node a = e1->source();
    ogdf::node b = e1->target();
    ogdf::node c = e2->source();
    ogdf::node d = e2->target();

    // If any of the edges share a node, return false immediately
    if (isSharedNode(a, b, c, d)) return false;

    // Calculate orientations for the pairs of points
    int o1 = orientation(a, b, c);
    int o2 = orientation(a, b, d);
    int o3 = orientation(c, d, a);
    int o4 = orientation(c, d, b);

    // If orientations are different, the edges intersect
    if (o1 != o2 && o3 != o4) return true;

    // Handle special cases where points are collinear but lie on the segment
    // Perform this check only if the edges are collinear
    if (o1 == 0) {
        if (onSegment(a, b, c) || onSegment(a, b, d)) return true;
    }
    if (o2 == 0) {
        if (onSegment(a, b, d)) return true;
    }
    if (o3 == 0) {
        if (onSegment(c, d, a)) return true;
    }
    if (o4 == 0) {
        if (onSegment(c, d, b)) return true;
    }

    // Additional check for collinear segments that just touch at one endpoint
    // Ensure segments are touching at one endpoint or overlap slightly
    if (o1 == 0 && o2 == 0 && o3 == 0 && o4 == 0) {
        // Ensure segments are touching at one endpoint
        if ((onSegment(a, b, c) && onSegment(a, b, d)) || 
            (onSegment(c, d, a) && onSegment(c, d, b))) {
            return true;
        }
    }

    return false; // No intersection
}
