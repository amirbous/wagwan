#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/energybased/FMMMLayout.h>

#include "../include/IO.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/algorithm/rearrangeGraph.hpp"
#include "../include/algorithm/simulated_annealing.hpp"

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

    auto start_intersect_before = std::chrono::high_resolution_clock::now();
    std::map<int, ogdf::edge, std::greater<int>> intersectionCountBefore = calculate_singular_intersections(findIntersections(G,GA));
    auto end_intersect_before = std::chrono::high_resolution_clock::now();

    int max_intersect_before = intersectionCountBefore.begin()->first;

    std::cout <<fileName << std::endl;
    std::cout << "before: " << max_intersect_before <<std::endl;


    std::set<std::pair<int, int>> occupiedPositions;

    auto start_preprocess = std::chrono::high_resolution_clock::now();

    FMMMLayout layout;
    layout.useHighLevelOptions(true);
    layout.unitEdgeLength(width / 50.0); 
    layout.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
    layout.call(GA);

    adjustCoordinatesToGrid(G, GA, occupiedPositions, static_cast<double>(width), static_cast<double>(height));

    auto end_preprocess = std::chrono::high_resolution_clock::now();


    //simulated_annealing(G,GA,nodesId,30,5);
    
    auto end_annealing = std::chrono::high_resolution_clock::now();


    std::map<int, ogdf::edge, std::greater<int>> intersectionCountAfter = calculate_singular_intersections(findIntersections(G,GA));

    int max_intersect_after = intersectionCountAfter.begin()->first;


    auto end_intersect_after =  std::chrono::high_resolution_clock::now();
    std::cout << "After: " <<  max_intersect_after <<std::endl;

    std::cout << "Time to evaluate intersections before: " <<  std::chrono::duration_cast<std::chrono::microseconds>(end_intersect_before - start_intersect_before).count() << "µs" <<std::endl;
    std::cout << "Time to preprocess: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end_preprocess - start_preprocess).count() << "µs" << std::endl;
    std::cout << "Time to annealing: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end_annealing - end_preprocess).count() << "µs" << std::endl;
    std::cout << "Time to evaluate intersections after: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end_intersect_after - end_annealing).count() << "µs" << std::endl;


    writeGraphToJson(G, GA, outFile, nodesId, width, height);

	return 0;
}
