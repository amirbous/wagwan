#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>


#include "../include/algorithm/linesweeper.hpp"
#include "../include/intersectGraph.hpp"

#include <unordered_set>
#include <vector>
#include <set>



// overload of linesweeper find intersections, nested loops, under the assumption that length of edges is way less than all of edges
std::vector<std::pair<ogdf::edge, ogdf::edge>> findIntersections(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, std::vector<ogdf::edge> edges) {
    std::vector<std::pair<ogdf::edge, ogdf::edge>> intersectinos;
    for (auto & graph_edge : G.edges) 
    {
        for (auto & incident_edge : edges) {
            if (edgesIntersect(GA, graph_edge, incident_edge)) {
                intersectinos.push_back({graph_edge, incident_edge});
            }
        }
    }
    return intersectinos;

}

struct Event {
    double x;
    bool isStart;
    ogdf::edge edge;

    bool operator<(const Event &other) const {
        if (x != other.x) return x < other.x;
        if (isStart != other.isStart) return !isStart; // End events processed before start
    return edge->index() < other.edge->index(); // Tie-breaking by edge index
}
};




// Adjust findIntersections to recheck the active set
std::vector<std::pair<ogdf::edge, ogdf::edge>> findIntersections(const ogdf::Graph &G, const ogdf::GraphAttributes &GA) {
    std::vector<Event> events;

    // Create events for each edge
    for (ogdf::edge e : G.edges) {
        ogdf::node p1 = e->source();
        ogdf::node p2 = e->target();

        if (GA.x(p1) > GA.x(p2)) std::swap(p1, p2);

        events.push_back({GA.x(p1), true, e});  // Start event
        events.push_back({GA.x(p2), false, e}); // End event
    }

    std::sort(events.begin(), events.end());

    std::set<ogdf::edge> activeSet;
    std::set<std::pair<int, int>> uniqueIntersections;
    std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections;

    // Process each event in sorted order
    for (const auto &event : events) {
        ogdf::edge edge = event.edge;

        if (event.isStart) {
            // Check for intersections with active edges
            for (ogdf::edge activeEdge : activeSet) {
                // Capture GA by value to avoid dangling reference issues
                if (edgesIntersect(GA, edge, activeEdge)) {
                    auto edgePair = std::make_pair(std::min(edge->index(), activeEdge->index()),
                               std::max(edge->index(), activeEdge->index()));
                    if (uniqueIntersections.insert(edgePair).second) {
                        intersections.emplace_back(edge, activeEdge);
                    }
                }
            }
            activeSet.insert(edge); // Add current edge to active set
        } else {
            activeSet.erase(edge); // Remove the edge from active set
        }
    }

    return intersections;
}


std::map<int, ogdf::edge, std::greater<int>> calculate_singular_intersections(const std::vector<std::pair<ogdf::edge,ogdf::edge>> &edges)
{
    std::map<ogdf::edge, int> edge_to_intersect;
    std::map<int, ogdf::edge, std::greater<int>> intersection_count;

    for (auto edge : edges)
    {
        edge_to_intersect[edge.first]++;
        edge_to_intersect[edge.second]++;
    }
    for (auto edge : edge_to_intersect) {
        intersection_count[edge.second] = edge.first;
    }
    return intersection_count;
}

int calculate_specific_intersections(const std::vector<std::pair<ogdf::edge,ogdf::edge>> &edges, const ogdf::edge &specific_edge)
{
    int count = 0;
    for (auto &edge : edges)
        if (edge.first == specific_edge || edge.second == specific_edge)
            count++;
    return count;
}
