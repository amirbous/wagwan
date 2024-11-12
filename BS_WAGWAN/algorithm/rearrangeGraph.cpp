#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include<set>
#include <cmath>

#include "../include/algorithm/rearrangeGraph.hpp"



void adjustCoordinatesToGrid(ogdf::Graph &G, ogdf::GraphAttributes &GA, double gridWidth, double gridHeight) {
    for (ogdf::node u : G.nodes) {
        double x = GA.x(u);
        double y = GA.y(u);

        x = std::max(0.0, std::min(x, gridWidth));
        y = std::max(0.0, std::min(y, gridHeight));

        GA.x(u) = x;
        GA.y(u) = y;
    }
}

void rearrangeToIntGraph(ogdf::Graph &G, ogdf::GraphAttributes &GA) {

    std::set<std::pair<int, int>> occupiedPosition;
    for (ogdf::node u: G.nodes) {

        bool inserted = false;

        float x = GA.x(u);
        float y = GA.y(u);

        bool xIsInt = x == floor(x);
        bool yIsInt = y == floor(y);

        if (xIsInt && yIsInt) {
            occupiedPosition.insert({static_cast<int>(x), static_cast<int>(y)});
            inserted = true;
        }

        int xDown = static_cast<int>(std::floor(x));
        int xUp = static_cast<int>(std::ceil(x));
        int yDown = static_cast<int>(std::floor(y));
        int yUp = static_cast<int>(std::ceil(y));

        std::vector<std::pair<int, int>> candidates = {
        {xDown, yDown},
        {xDown, yUp},
        {xUp, yDown},
        {xUp, yUp}
        };

        for (const auto &[newX, newY] : candidates) {
            if (!occupiedPosition.count({newX, newY})) {
                // Set node position and mark it as occupied
                GA.x(u) = static_cast<float>(newX);
                GA.y(u) = static_cast<float>(newY);
                occupiedPosition.insert({newX, newY});
                inserted = true;
                break;
            }
        }



    }

}