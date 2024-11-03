#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <algorithm>
#include <map>  
#include <string>
#include <tuple>
#include <vector>

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

        Node operator-(const Node& node) const 
        { 
            // don't remove -1, compiler doesn't recognise instance
            return {-1, this->x - node.x, this->y - node.y}; }

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
        void setX(int x)
        {
            this->x = x;
        }
        void setY(int y) 
        {
            this->y = y;
        }
        void updateLocation (int x, int y) {
            this->x = x;
            this->y = y;
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
        // ! Needed for map definition
        bool operator<(const Edge& other) const {
        if (this->source_id != other.source_id) {
            return this->source_id < other.source_id;
        }
        return this->target_id < other.target_id;
    }

    // getters and setters  
        int getSource_id() const
        {
            return this->source_id;
        }

        int getTarget_id() const
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
        std::map<Edge, int> edges;
        Grid grid{};
    public:
        // constructors    
        Graph() {
            //initialise with empty nodes and edges
            this->nodes = std::vector<Node>{};
            this->edges = std::map<Edge, int>{};
            this->grid = Grid{};


        };
        Graph(std::vector<Node> nodes, std::map<Edge, int> edges, Grid grid)
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

        std::map<Edge, int> getEdges()
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

        void setEdges(std::map<Edge, int> edges)
        {
            this->edges = edges;
        }



        Node& getNode(int id) {
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
        const Edge& getEdge(int source_id, int target_id) {
            for (const auto& [edge, intersectionCount] : this->edges) {
                if (edge.getSource_id() == source_id && edge.getTarget_id() == target_id) {
                    return edge;  
                }
            }
            std::cout << "Edge with source id " << source_id << " and target id " << target_id << " not found!" << std::endl;
            exit(-1);
        }
        void UpdateNodeLocation(int id, int x, int y) {
            Node& node = this->getNode(id);
            node.setX(x);
            node.setY(y);
            
        }

        // other functions
        void getStats() {
            std::cout << "The graph has " << nodes.size() <<
                " nodes and " << edges.size() <<
                " edges" << std::endl;

        }

        


};


#endif