#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <algorithm>
#include <iostream>

#include "../include/IO.hpp"

class Node{
    private:
        int id;
        int x;
        int y;

    public:
        // constructors
        Node (int id = -1, int x = -1, int y = -1) {
            this->id = id;
            this->x = x;
            this->y = y;
        }

        Node operator-(const Node& p) const { return Node(this->x - p.x, this->y - p.y); }

        //getters and setters
        int getId()
        {
            return this->id;
        }

        int getX()
        {
            return this->x;
        }

        int getY()
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
        int getSource_id()
        {
            return this->source_id;
        }

        int getTarget_id()
        {
            return this->target_id;
        }

        void setSource_id(int source_id)
        {
            this->source_id = source_id;
        }

        void setTarget_id(int target_id)
        {
            this->target_id = target_id;
        }
};

struct Grid {
    Grid() {
        width = 0;
        height = 0;
    }
    int width;
    int height;
};

class Graph{
    private:
        //attributes, set of nodes and edges
        std::vector<Node> nodes;
        std::vector<Edge> edges;
        Grid grid{};
    public:
        // constructors    
        Graph() {
            //initialise with empty nodes and edges
            this->nodes = std::vector<Node>{};
            this->edges = std::vector<Edge>{};
            this->grid = Grid{};


        };
        Graph(std::vector<Node> nodes, std::vector<Edge> edges, Grid grid)
        : grid{}
        
        {
            this->nodes = nodes;
            this->edges = edges;
            this->grid = grid;  
        };

        // getters and setters
        std::vector<Node> getNodes()
        {
            return this->nodes;
        }

        std::vector<Edge> getEdges()
        {
            return this->edges;
        }

        Grid getGrid() 
        {
            return this->grid;
        }
        void setNodes(std::vector<Node> nodes)
        {
            this->nodes = nodes;
        }

        void setEdges(std::vector<Edge> edges)
        {
            this->edges = edges;
        }


        Node getNode(int id) {
            auto it = std::find_if(this->nodes.begin(), this->nodes.end(), [id](Node node){return id == node.getId();});
            if (it != nodes.end()) {
                return *it;
            }
            else {
                std::cout << "No node with id " << id << " has been found!" << std::endl
                    << "aborting" << std::endl;
                    exit(-1);
            }
        }

        // other functions
        void getStats() {
            std::cout << "The graph has " << nodes.size() <<
                " nodes and " << edges.size() <<
                "edges" << std::endl;

        }


};


#endif