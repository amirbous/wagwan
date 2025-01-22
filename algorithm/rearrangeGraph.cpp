#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include <set>
#include <utility>
#include <queue>

#include <cmath>

#include <algorithm>

#include "../include/algorithm/rearrangeGraph.hpp"
#include "../include/intersectGraph.hpp"

bool OnGridWithRadius (std::pair<int, int> position, int radius, int gridWidth, int gridHeight) {

    return position.first - radius >= 0 || position.first + radius < gridWidth
        || position.second - radius >= 0 || position.second + radius < gridHeight;
}

bool PositionOnGrid (std::pair<int, int> position, int gridWidth, int gridHeight) {
    return position.first > 0 && position.first <= gridWidth
        && position.second > 0 && position.second <= gridHeight; 
}

std::vector<std::pair<int, int>> generateFramePositions(int xDown, int yDown, int lookup_radius) {

    std::vector<std::pair<int, int>> frame;

    int sideLength = lookup_radius*2 + 1;
    std::vector<int> rangeX(sideLength);
    std::iota(rangeX.begin(), rangeX.end(), xDown); 
    
    std::vector<int> rangeY(sideLength);
    std::iota(rangeY.begin(), rangeY.end(), yDown);

    for (int i = xDown; i < xDown + sideLength; i++) {
        if (i == xDown || i == xDown + sideLength - 1) {
            for (int j = yDown; j < yDown + sideLength; j++) {
                frame.push_back(std::pair<int, int>{i, j});
            }
        }
        else {
            frame.push_back(std::pair<int, int>{i, yDown});
            frame.push_back(std::pair<int, int>{i, yDown + sideLength - 1});
        }

    }
    return frame;


}

void EmplaceWithinLookup(ogdf::Graph &G, ogdf::GraphAttributes &GA,
                        ogdf::node& u, std::set<std::pair<int, int>>& populatedPositions,
                        double gridWidth, double gridHeight) {


    double x = GA.x(u);
    double y = GA.y(u);

    auto rng = std::default_random_engine {};
    int lookup_radius = 0;
    int xDown = static_cast<int>(std::floor(x - lookup_radius)), yDown = static_cast<int>(std::floor(y - lookup_radius));
    bool stillOnGrid = OnGridWithRadius(std::pair<int, int>{x, y}, lookup_radius, gridWidth, gridHeight);
    //int i = 0;
    while (stillOnGrid) {
    //    std::cout << "i edges = " << i << ";";
    //    i++;
        int xDown = static_cast<int>(std::floor(x - lookup_radius));
        int yDown = static_cast<int>(std::floor(y - lookup_radius));
        
       std::vector<std::pair<int, int>> candidates = generateFramePositions(xDown, yDown, lookup_radius);
       std::shuffle(candidates.begin(), candidates.end(), rng);
        for (const auto& pos : candidates) {
            if (populatedPositions.find(pos) == populatedPositions.end() && 
                    PositionOnGrid(pos, gridWidth, gridHeight)) {
                GA.x(u) = (pos.first);
                GA.y(u) = (pos.second);

                populatedPositions.insert(std::pair<int, int>{pos});

                return;
            }
        }
        
        lookup_radius++;
        stillOnGrid = OnGridWithRadius(std::pair<int, int>{x, y}, lookup_radius, gridWidth, gridHeight);
    }






}


void resolveNodeOnEdge(ogdf::Graph &G, ogdf::GraphAttributes &GA,
                        std::set<std::pair<int, int>>& populatedPositions, ogdf::node u,
                        double gridWidth, double gridHeight) {
    int x = (int)GA.x(u);
    int y = (int)GA.y(u);

    auto rng = std::default_random_engine {};     
    int lookup_radius = 0;

    bool stillOnGrid = OnGridWithRadius(std::pair<int, int>{x, y}, lookup_radius, gridWidth, gridHeight);
    //int i = 0;
    while (stillOnGrid) {
        //std::cout << "i = " << i << ";";
        //i++;
        int xDown = x - lookup_radius;
        int yDown = y - lookup_radius;


       std::vector<std::pair<int, int>> candidates = generateFramePositions(xDown, yDown, lookup_radius);
       std::shuffle(candidates.begin(), candidates.end(), rng);
        for (const auto& pos : candidates) {
            double old_x = GA.x(u);
            double old_y = GA.y(u);

            // need to update to check if intersecting any edges
            GA.x(u) = pos.first;
            GA.y(u) = pos.second;

            // check node on any edge
            bool posOnEdge = check_node_on_anyEdge(G, GA, u);

            // incident edges of node intersect any other edges
            bool incidentEdgesOnNode = nodeEdges_intersect_anyNode(G, GA, u);

            // revert after checks
            GA.x(u) = old_x;
            GA.y(u) = old_y;
 
            if (populatedPositions.find(pos) == populatedPositions.end() && 
                    PositionOnGrid(pos, gridWidth, gridHeight) && !posOnEdge && !incidentEdgesOnNode) {
                populatedPositions.erase({old_x, old_y});
                GA.x(u) = (pos.first);
                GA.y(u) = (pos.second);
                populatedPositions.insert({pos.first, pos.second});

                return;
            }
        }
        
        lookup_radius++;
        stillOnGrid = OnGridWithRadius(std::pair<int, int>{x, y}, lookup_radius, gridWidth, gridHeight);
    }   
        

    }

void adjustCoordinatesToGrid(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
                            std::set<std::pair<int, int>>& populatedPositions, 
                            double gridWidth, double gridHeight) {

     
    
    std::vector<ogdf::node> allNodes;
    for (const auto & n : G.nodes) {
        allNodes.push_back(n);
    }
    int max_x = GA.x(*std::max_element(allNodes.begin(), allNodes.end(), [GA](ogdf::node n1, ogdf::node n2){return GA.x(n1) < GA.y(n2);}));
    int max_y = GA.y(*std::max_element(allNodes.begin(), allNodes.end(), [GA](ogdf::node n1, ogdf::node n2){return GA.y(n1) < GA.y(n2);}));


    int min_x = GA.x(*std::min_element(allNodes.begin(), allNodes.end(), [GA](ogdf::node n1, ogdf::node n2){return GA.x(n1) < GA.x(n2);}));
    int min_y = GA.y(*std::min_element(allNodes.begin(), allNodes.end(), [GA](ogdf::node n1, ogdf::node n2){return GA.y(n1) < GA.y(n2);}));

    int oldRangeX = max_x - min_x;
    int oldRangeY = max_y - min_y;

    for (ogdf::node u : G.nodes) {
        double newX = (GA.x(u) - min_x)*gridWidth / (oldRangeX);
        double newY = (GA.y(u) - min_y)*gridHeight / (oldRangeY);

        GA.x(u) = newX;
        GA.y(u) = newY;

        EmplaceWithinLookup(G, GA, u, populatedPositions, gridWidth, gridHeight);

    }
    for (const auto  &u : G.nodes) {
        if (check_node_on_anyEdge(G, GA, u)) {
            resolveNodeOnEdge(G, GA, populatedPositions, u, gridWidth, gridHeight);
        }
    }

}


void centerInGrid(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
                  double gridWidth, double gridHeight) {


    double sumX = 0, sumY = 0;
    int nodeCount = 0;

    for (ogdf::node u : G.nodes) {
        sumX += GA.x(u);
        sumY += GA.y(u);
        nodeCount++;
    }

    if (nodeCount == 0) return; 

    double centerX = sumX / nodeCount;
    double centerY = sumY / nodeCount;


    double gridCenterX = gridWidth / 2.0;
    double gridCenterY = gridHeight / 2.0;


    double offsetX = gridCenterX - centerX;
    double offsetY = gridCenterY - centerY;


    for (ogdf::node u : G.nodes) {
        double newX = GA.x(u) + offsetX;
        double newY = GA.y(u) + offsetY;


        GA.x(u) = newX;
        GA.y(u) = newY;
    }
}