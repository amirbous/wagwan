#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <fstream>
#include <unordered_map>
#include "../include/json.hpp"
#include "../include/simdjson.h"

#include <iostream>
#include <string>

#include "../include/IO.hpp"

using json = nlohmann::json;

using namespace ogdf;

void initializeGraphFromJson(Graph &G, GraphAttributes &GA, const std::string &filePath, std::unordered_map<node, int>& nodesId, int& width, int& height) {
 


    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(filePath);
    simdjson::ondemand::document doc = parser.iterate(json);

    std::cout << "File " << filePath << " has been loaded succesfully!" << std::endl;

    // Map to store node IDs and their corresponding node references in OGDF
    std::unordered_map<int, node> nodeMap;
    // Add nodes to the graph with attributes
    for (auto jsonNode : doc["nodes"]) {
        int nodeId = (jsonNode["id"]).get_int64();
        double x = (jsonNode["x"]).get_double();
        double y = (jsonNode["y"]).get_double();

        node u = G.newNode();

        GA.shape(u) = Shape::Ellipse;
        nodeMap[nodeId] = u;
        nodesId[u] = nodeId;
        // Set node coordinates in GraphAttributes
        GA.x(u) = x;
        GA.y(u) = y;
        GA.height(u) = 10;
        GA.width(u) = 10;

    }

    // Add edges to the graph based on source and target IDs
    for (auto jsonEdge : doc["edges"]) {
        int sourceId = (jsonEdge["source"]).get_int64();
        int targetId = (jsonEdge["target"]).get_int64();

        node u = nodeMap[sourceId];
        node v = nodeMap[targetId];


        G.newEdge(u, v);
    }

    width = doc["width"].get_int64();
    height = doc["height"].get_int64();

}


void writeGraphToJson(Graph &G, GraphAttributes &GA, const std::string &filePath,
        std::unordered_map<node, int>& nodesId, int width, int height) {

    nlohmann::json graphJson;

    for (ogdf::node u : G.nodes) {
        nlohmann::json jsonNode;
        jsonNode["id"] = nodesId[u];
        jsonNode["x"] = static_cast<int>(GA.x(u));  // Round and cast to int
        jsonNode["y"] = static_cast<int>(GA.y(u));  // Round and cast to int
        graphJson["nodes"].push_back(jsonNode);
    }

    for (ogdf::edge e : G.edges) {
        nlohmann::json jsonEdge;
        jsonEdge["source"] = nodesId[e->source()];
        jsonEdge["target"] = nodesId[e->target()];
        graphJson["edges"].push_back(jsonEdge);
    }

    graphJson["width"] = width;
    graphJson["height"] = height;

    std::ofstream outFile(filePath);
    outFile << graphJson.dump(4);  
    outFile.close();

    std::cout << "Graph written to " << filePath << " successfully!" << std::endl;
}

void writeGraphToWagwan(Graph &G, GraphAttributes &GA, const std::string &filePath, 
        std::unordered_map<node, int>& nodesId,  int width, int height, ogdf::edge worst_edge) {
    nlohmann::json graphJson;

    for (ogdf::node u : G.nodes) {
        nlohmann::json jsonNode;
        jsonNode["id"] = nodesId[u];
        jsonNode["x"] = static_cast<int>(GA.x(u));  // Round and cast to int
        jsonNode["y"] = static_cast<int>(GA.y(u));  // Round and cast to int
        graphJson["nodes"].push_back(jsonNode);
    }

    for (ogdf::edge e : G.edges) {
        nlohmann::json jsonEdge;
        jsonEdge["source"] = nodesId[e->source()];
        jsonEdge["target"] = nodesId[e->target()];
        graphJson["edges"].push_back(jsonEdge);
    }

    graphJson["width"] = width;
    graphJson["height"] = height;
    
    
    graphJson["worst_edge"].push_back(
        {
            {"source", nodesId[worst_edge->source()]},
            {"target", nodesId[worst_edge->target()]}
        }
    );
    std::ofstream outFile(filePath);
    outFile << graphJson.dump(4);  
    outFile.close();

    std::cout << "Graph written to " << filePath << " successfully!" << std::endl;

}

