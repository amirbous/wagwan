#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "../include/json.hpp"
#include "../include/IO.hpp"
#include "../include/simdjson.h"
#include "../include/graph.hpp"

using json = nlohmann::json;

class Node;
class Graph;
class Edge;

/**
 * @ref git@github.com:simdjson/simdjson.git
 * Reads graph from file
 * 
 * reads .json file specified by the competition website and returns a graph
 *
 * @param filename: name of file to read graph from 
 * @return graph instance specified in the file
 */
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

    //std::map<Edge, int> edges;
    std::map<Edge, int> edges;
    // parse edges
    for (auto edge: simdjson::ondemand::array(doc["edges"]))
    {
        Edge current_edge(edge["source"].get_int64(), edge["target"].get_int64());
        edges.insert(std::make_pair(current_edge, 0));
        
    }
    return {nodes, edges, Grid{}};
}

bool writeGraph(std::string filename, Graph graph){

    std::cout << "writing graph to file " << filename << std::endl;  
    nlohmann::json json_graph;

    std::vector<Node> nodes = graph.getNodes();
    std::map<Edge, int> edges = graph.getEdges();

    // add nodes from the graph to the nodes array in the json
    for (auto node: nodes)
    {
      json_graph["nodes"].push_back({
          {"id", node.getId(),},
          {"x", node.getX()},
          {"y", node.getY()}
      });
    }

    //add edges from the graph to the edges array in the json
    for (const auto& [edge, intersectionCount] : edges)
    {
      json_graph["edges"].push_back({
        {"source", edge.getSource_id()},
        {"target", edge.getTarget_id()}
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
