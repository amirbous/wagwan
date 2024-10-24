#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

#include "../include/IO.hpp"
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
        int get_id()
        {
            return this->id;
        }

        int get_x()
        {
            return this->x;
        }

        int get_y()
        {
            return this->y;
        }

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
    int get_source_id()
    {
        return this->source_id;
    }

    int get_target_id()
    {
        return this->target_id;
    }

    void set_source_id(int source_id)
    {
        this->source_id = source_id;
    }

    void set_target_id(int target_id)
    {
        this->target_id = target_id;
    }
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
        std::vector<Node> get_nodes()
        {
            return this->nodes;
        }

        std::vector<Edge> get_edges()
        {
            return this->edges;
        }

        void set_nodes(std::vector<Node> nodes)
        {
            this->nodes = nodes;
        }

        void set_edges(std::vector<Edge> edges)
        {
            this->edges = edges;
        }

        // other functions
        void getStats() {
            std::cout << "The graph has " << nodes.size() <<
                "and " << edges.size();

        }


};


#endif