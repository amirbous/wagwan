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
    for (const auto &edge : example.getEdges()) {
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
    }


    writeGraph("output.json", example);


}
