#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../include/json.hpp"
#include "../include/IO.hpp"
#include "../include/simdjson.h"
#include "../include/graph.hpp"

using json = nlohmann::json;

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

    return {nodes, edges};
}

bool writeGraph(std::string filename, Graph graph){

    std::cout << "writing graph to file " << filename << std::endl;  
    graph.getStats();

    nlohmann::json json_graph;

    std::vector<Node> nodes = graph.get_nodes();
    std::vector<Edge> edges = graph.get_edges();

    // add nodes from the graph to the nodes array in the json
    for (auto node: nodes)
    {
      json_graph["nodes"].push_back({
          {"id", node.get_id(),},
          {"x", node.get_x()},
          {"y", node.get_y()}
      });
    }

    //add edges from the graph to the edges array in the json
    for (auto edge: edges)
    {
      json_graph["edges"].push_back({
        {"from", edge.get_source_id()},
        {"to", edge.get_target_id()}
      });
    }

    // copy the json object into th file
    std::ofstream file(filename);
    if (file.is_open())
    {
      file << json_graph.dump(4);
      file.close();
      std::cout << "Succesfully graph written to " << filename << std::endl;
      return true;
    }

    return false;
}
