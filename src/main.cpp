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

#include "../include/algorithm/simulated_annealing.hpp"

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

    //simulated_annealing(G,GA,nodesId,1,1);

    std::cout<<calculate_singular_intersections(findIntersections(G,GA))[0].first<<std::endl;

    writeGraphToJson(G,GA,outFile,nodesId,height,width);



    /*std::set<std::pair<int, int>> occupiedPositions{};


    std::cout << std::endl;
    FMMMLayout layout;
    layout.useHighLevelOptions(true);
    layout.unitEdgeLength(width / 100.0); 
    layout.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
    layout.call(GA);  




    adjustCoordinatesToGrid(G, GA, occupiedPositions, static_cast<double>(width), static_cast<double>(height));


 //   std::cout << max_intersect;
    writeGraphToJson(G, GA, outFile, nodesId, width, height);*/



	return 0;
}