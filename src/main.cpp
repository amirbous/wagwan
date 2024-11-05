#include <iostream>

#include "../include/graph.hpp"
#include "../include/IO.hpp"
#include "../include/utils.hpp"

int main() {


	std::cout << "Welcome to Wagwan" << std::endl;
	std::cout <<"This code runs fine" << std::endl;

    Graph example = readGraph("resources/ex3_k0_xr0.json");

    // TODO: verify output of this result manually or by writing some tests
    // TODO: add parameters

    // 
    /*for (const auto &edge : example.getEdges()) {
        const Edge graphEdge = edge.first;
        for (const auto &inneredge : example.getEdges()) {
            const Edge& graphInnerEdge = inneredge.first;
            if (edgesIntersect(example, graphEdge, graphInnerEdge)) {
                std::cout << "edge { (" << graphEdge.getSource_id() << ", " 
                << graphEdge.getTarget_id() << ")"
                << ", (" << graphInnerEdge.getSource_id() << ", " 
                << graphInnerEdge.getTarget_id() << ") }" << std::endl;
            }

        }
    }*/
    std::vector<std::pair<Edge,Edge>> intersections = findIntersections(example);
    std::map<Edge,int> intersection_count;
    for(std::pair<Edge,Edge> edges : intersections)
    {
        intersection_count[edges.first]++;
        intersection_count[edges.second]++;
    }

    for(auto count: intersection_count)
    {
        std::cout << "Edge from " << count.first.getSource_id() << " to " << count.first.getTarget_id() << " intersects a total of " << count.second << " times." << std::endl;
    }

    writeGraph("output.json", example);


}
