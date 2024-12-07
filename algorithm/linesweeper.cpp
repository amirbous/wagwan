#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>


#include "../include/algorithm/linesweeper.hpp"
#include "../include/intersectGraph.hpp"

#include <unordered_set>
#include <vector>
#include <set>


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

    for (const auto &event : events) {
        ogdf::edge edge = event.edge;

        if (event.isStart) {
            for (ogdf::edge activeEdge : activeSet) {
                if (edgesIntersect(GA, edge, activeEdge)) {
                    auto edgePair = std::minmax(edge->index(), activeEdge->index());
                    if (uniqueIntersections.insert(edgePair).second) {
                        intersections.emplace_back(edge, activeEdge);
                    }
                }
            }
            activeSet.insert(edge);
        } else {
            activeSet.erase(edge);
        }
    }

    return intersections;
}

std::map<ogdf::edge, int> calculate_singular_intersections(const std::vector<std::pair<ogdf::edge,ogdf::edge>> &edges)
{
    std::map<ogdf::edge,int> intersection_count;
    for (auto edge : edges)
    {
        intersection_count[edge.first]++;
        intersection_count[edge.second]++;
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
