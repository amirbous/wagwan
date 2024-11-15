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
#include <set>

using namespace ogdf;

int main(int argc, char** argv)
{

    if (argc < 3) {
        std::cout << "requires 2 arguments: \n input file \n output file" << std::endl;
        exit(-1);
    }
    const std::string fileName = argv[1];
    const std::string outFile = argv[2];

	Graph G;

    GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);

    std::unordered_map<node, int> nodesId;

    
    int height, width;

    initializeGraphFromJson(G, GA, fileName, nodesId, width, height);

    double dwidth = static_cast<double>(width), dheight = static_cast<double>(height);

   /* std::vector<std::pair<edge, edge>> intersections = findIntersections(G, GA);



    std::map<edge, int>  intersectionCount;

    int max_intersect = 0;
    ogdf::edge e_max{};
    
    for(std::pair<edge,edge> edges : intersections)
    {
        intersectionCount[edges.first]++;
        intersectionCount[edges.second]++;
    }
    for (const auto& pair : intersectionCount) {
        std::cout << pair.second << "\n";

    }*/



    std::set<std::pair<int, int>> occupiedPositions{};


    std::cout << std::endl;
    FMMMLayout layout;
    layout.useHighLevelOptions(true);
    layout.unitEdgeLength(width / 100.0); 
    layout.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
    layout.call(GA);  




    adjustCoordinatesToGrid(G, GA, occupiedPositions, static_cast<double>(width), static_cast<double>(height));


 //   std::cout << max_intersect;
    writeGraphToJson(G, GA, outFile, nodesId, width, height);



	return 0;
}