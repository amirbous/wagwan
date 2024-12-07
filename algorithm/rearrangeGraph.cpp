#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include <set>
#include <utility>

#include <cmath>

#include <algorithm>

#include "../include/algorithm/rearrangeGraph.hpp"

bool OnGridWithRadius (std::pair<int, int> position, int radius, int gridWidth, int gridHeight) {

    return position.first - radius >= 0 || position.first + radius <= gridWidth
        || position.second - radius >= 0 || position.second + radius <= gridHeight;
}

bool PositionOnGrid (std::pair<int, int> position, int gridWidth, int gridHeight) {
    return position.first > 0 && position.first <= gridWidth
        && position.second > 0 && position.second <= gridHeight; 
}

std::vector<std::pair<int, int>> generateFramePositions(int xDown, int yDown, int lookup_radius) {

    std::vector<std::pair<int, int>> frame;

    int sideLength = lookup_radius*2 + 2;
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
    while (stillOnGrid) {
       
        int xDown = static_cast<int>(std::floor(x - lookup_radius));
        int yDown = static_cast<int>(std::floor(y - lookup_radius));
        
       std::vector<std::pair<int, int>> candidates = generateFramePositions(xDown, yDown, lookup_radius);
       std::shuffle(candidates.begin(), candidates.end(), rng);
        for (const auto& pos : candidates) {
            if (populatedPositions.find(pos) == populatedPositions.end() && 
                    PositionOnGrid(pos, gridWidth, gridHeight)) {

                GA.x(u) = static_cast<double>(pos.first);
                GA.y(u) = static_cast<double>(pos.second);
                populatedPositions.insert(std::pair<int, int>{pos});
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

    for (ogdf::node u : G.nodes) {
        double x = GA.x(u);
        double y = GA.y(u);

        GA.x(u) = std::min(x, gridWidth);
        GA.y(u) = std::min(y, gridHeight);

        EmplaceWithinLookup(G, GA, u, populatedPositions, gridWidth, gridHeight);

    }
}

void rearrangeToIntGraph(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
    std::set<std::pair<int, int>> occupiedPositions, double gridWidth, double gridHeight) {

    std::set<std::pair<int, int>> occupiedPosition;


    for (ogdf::node u: G.nodes) {

        EmplaceWithinLookup(G, GA, u, occupiedPosition, gridWidth, gridHeight); 
 
    }

}
    
void centerInGrid(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
                  std::set<std::pair<int, int>>& populatedPositions, 
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

        EmplaceWithinLookup(G, GA, u, populatedPositions, gridWidth, gridHeight);
    }
}