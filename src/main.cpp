#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/graphalg/PlanarSeparatorModule.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <ogdf/basic/LayoutStatistics.h>
#include <ogdf/fileformats/GraphIO.h>

#include "../include/IO.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/algorithm/rearrangeGraph.hpp"
#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/intersectGraph.hpp"

#include <string>
#include <vector>

#include <map>
#include <set>

#include <math.h>

#include <iostream>



int main(int argc, char** argv)
{

    if (argc < 4) {
        std::cout << "requires 3 arguments: \n input file \n output file \n cooling_technique(0, 1 or 2)" << std::endl;
        exit(-1);
    }
    
    const std::string fileName = argv[1];
    const std::string outFile = argv[2];
    const int cooling_technique = std::stoi(argv[3]);

	Graph G;

    GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);

    std::unordered_map<node, int> nodesId;

    
    int height, width;

    initializeGraphFromJson(G, GA, fileName, nodesId, width, height);



    auto start_intersect_before = std::chrono::high_resolution_clock::now();
    std::map<int, ogdf::edge, std::greater<int>> intersectionCountBefore = calculate_singular_intersections(findIntersections(G,GA));
    auto end_intersect_before = std::chrono::high_resolution_clock::now();

    int max_intersect_before = intersectionCountBefore.begin()->first;
    intersectionCountBefore.clear(); 


    std::cout << max_intersect_before << std::endl;
    std::cout << "Time to evaluate intersections before: " <<  std::chrono::duration_cast<std::chrono::microseconds>(end_intersect_before - start_intersect_before).count() << "µs" <<std::endl;
    
    std::cout <<fileName << std::endl;
    std::cout << "before: " << max_intersect_before <<std::endl;


    std::map<std::pair<int, int>, bool> occupiedPositions;


    for (int x = 0; x <= width; ++x) {
        for (int y = 0; y <= height; ++y) {
            occupiedPositions[{x, y}] = false;
        }
    }

    auto start_preprocess = std::chrono::high_resolution_clock::now();



    FMMMLayout layout;
    layout.useHighLevelOptions(true);
    layout.unitEdgeLength(sqrt(pow(width, 2) + pow(height, 2)) / 20); 
    layout.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
    layout.call(GA);

    adjustCoordinatesToGrid(G, GA, occupiedPositions, static_cast<double>(width), static_cast<double>(height));

    std::map<int, ogdf::edge, std::greater<int>> afterPre = calculate_singular_intersections(findIntersections(G,GA));
    std::cout << "After Preprocessing" << afterPre.begin()->first << std::endl;


    auto end_preprocess = std::chrono::high_resolution_clock::now();

    if (afterPre.begin()->first < max_intersect_before) {
        writeGraphToJson(G, GA, outFile, nodesId, width, height);
    }
        
    simulated_annealing(G,GA,nodesId, 5000, width, height, cooling_technique, 1.0, 0.999, 
        std::min(afterPre.begin()->first, max_intersect_before), outFile);

    
    auto end_annealing = std::chrono::high_resolution_clock::now();


    std::map<int, ogdf::edge, std::greater<int>> intersectionCountAfter = calculate_singular_intersections(findIntersections(G,GA));

    int max_intersect_after = intersectionCountAfter.begin()->first;


    auto end_intersect_after =  std::chrono::high_resolution_clock::now();
    std::cout << "After: " <<  max_intersect_after <<std::endl;

    std::cout << "Time to evaluate intersections before: " <<  std::chrono::duration_cast<std::chrono::microseconds>(end_intersect_before - start_intersect_before).count() << "µs" <<std::endl;
    std::cout << "Time to preprocess: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end_preprocess - start_preprocess).count() << "µs" << std::endl;
    std::cout << "Time to annealing: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end_annealing - end_preprocess).count() << "µs" << std::endl;
    std::cout << "Time to evaluate intersections after: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end_intersect_after - end_annealing).count() << "µs" << std::endl;



	return 0;
}
