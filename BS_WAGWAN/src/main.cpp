#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/energybased/FMMMLayout.h>

#include "../include/IO.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/algorithm/rearrangeGraph.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace ogdf;

int main(int argc, char** argv)
{
    const std::string fileName = argv[1];

	Graph G;

    GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);

    std::unordered_map<node, int> nodesId;
    int height, width;

    initializeGraphFromJson(G, GA, fileName, nodesId, width, height);






    std::cout << std::endl;
    FMMMLayout layout;
    layout.useHighLevelOptions(true);
    layout.unitEdgeLength(50.0);  // Set the desired edge length
    layout.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
    layout.call(GA);  




    adjustCoordinatesToGrid(G, GA, static_cast<double>(width), static_cast<double>(height));
    rearrangeToIntGraph(G, GA);
    std::vector<std::pair<edge, edge>> intersections = findIntersections(G, GA);




    intersections = findIntersections(G, GA);

    std::map<edge, int>  intersectionCount;


    
    for(std::pair<edge,edge> edges : intersections)
    {
        intersectionCount[edges.first]++;
        intersectionCount[edges.second]++;
    }
    
    int max_intersect = 0;
    ogdf::edge e_max{};
    for (const auto& pair : intersectionCount) {
        if (pair.second > max_intersect) {
            max_intersect = pair.second;
            e_max = pair.first;
        }

    }

    writeGraphToWagwan(G, GA, "output.json", nodesId, width, height, e_max);



	return 0;
}