#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>

#include "IO.hpp"
#include <iostream>

class Node{
    private:
        int id;
        int x;
        int y;

    public:
        // constructors
        Node() {
            this->id = -1;
            this->x = -1;
            this->y = -1;
        };
        Node(int id, int x, int y) {
                this->id = id;
                this->x = x;
                this->y = y;

        }

        //getters and setters


};

class Edge{
    private:
        int source_id;
        int target_id;

    public:

    // constructrs
        Edge() {
            this->source_id = -1;
            this->target_id = -1;
        };
        Edge(int source_id, int target_id) {
            this->source_id = source_id;
            this->target_id = target_id;
        };

    // getters and setters  

};

class Graph{
    private:
        //attributes, set of nodes and edges
        std::vector<Node> nodes;
        std::vector<Edge> edges;

    public:
        // constructors    
        Graph() {
            //initialise with empty nodes and edges
            std::vector<Node> nodes;
            std::vector<Edge> edges;

            this->nodes = nodes;
            this->edges = edges;

        };
        Graph(std::vector<Node> nodes, std::vector<Edge> edges) {
            this->nodes = nodes;
            this->edges = edges;
        };

        // getters and setters

        // other functions
        void getStats() {
            std::cout << "The graph has " << nodes.size() <<
                "and " << edges.size();

        }


};


#endif