#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>


#include "../include/algorithm/linesweeper.hpp"
#include "../include/intersectGraph.hpp"


#include <vector>
#include <set>


struct Event {
    double x;
    bool isStart;
    ogdf::edge edge;

    bool operator<(const Event& other) const {
        return x < other.x || (x == other.x && isStart < other.isStart);
    }
};


std::vector<std::pair<ogdf::edge, ogdf::edge>> findIntersections(const ogdf::Graph &G, const ogdf::GraphAttributes &GA) {
    std::vector<Event> events;
    for (ogdf::edge e : G.edges) {
        auto p1 = e->source();
        auto p2 = e->target();

        if (GA.x(p1) > GA.x(p2)) std::swap(p1, p2);

        events.push_back({GA.x(p1), true, e});
        events.push_back({GA.x(p2), false, e});
    }

    std::sort(events.begin(), events.end());

    std::set<ogdf::edge> activeSet;
    std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections;

    for (const auto &event : events) {
        ogdf::edge edge = event.edge;

        if (event.isStart) {
            for (ogdf::edge activeEdge : activeSet) {
                if (edgesIntersect(GA, edge, activeEdge)) {
                    intersections.emplace_back(edge, activeEdge);
                }
            }
            activeSet.insert(edge);
        } else {
            activeSet.erase(edge);
        }
    }

    return intersections;
}