
#include "../include/graph.hpp"
#include "../include/graphIntersect.hpp"

#include <vector>
#include <set>


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
    for (const auto &edge : graph.getEdges()) {
        Node& p1 = graph.getNode(edge.first.getSource_id());
        Node& p2 = graph.getNode(edge.first.getTarget_id());

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