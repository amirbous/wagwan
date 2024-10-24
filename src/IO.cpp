#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../include/IO.hpp"
#include "../include/simdjson.h"
#include "../include/graph.hpp"


// important should be change just made placehoders to reduce compilation warnings
Graph readGraph(std::string filename) {
    std::cout << "reading graph from file " << filename << std::endl;

    // parse JSON
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(filename);
    simdjson::ondemand::document doc = parser.iterate(json);

    std::vector<Node> nodes;
    // parse nodes
    for (auto node: doc["nodes"])
    {
        Node current_node((node["id"]).get_int64(), node["x"].get_int64(), node["y"].get_int64());
        nodes.push_back(current_node);
    }

    std::vector<Edge> edges;
    // parse edges
    for (auto edge: simdjson::ondemand::array(doc["edges"]))
    {
        Edge current_edge(edge["from"].get_int64(), edge["to"].get_int64());
        edges.push_back(current_edge);
    }

    return Graph(nodes, edges);
}

bool writeGraph(std::string filename, Graph graph){

    std::cout << "writing graph to file " << filename << std::endl;  
    graph.getStats();

    return false;
}
