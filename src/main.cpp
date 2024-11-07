#include <iostream>

#include "../include/graph.hpp"
#include "../include/IO.hpp"
#include "../include/graphIntersect.hpp"
#include "../include/algorithms/lineSweeper.hpp"
void printLocation(Node node) 
{
    std::cout <<" (" << node.getX() << ", " << node.getY() << ") ";
}
int main() {


	std::cout << "Welcome to Wagwan" << std::endl;
	std::cout <<"This code runs fine" << std::endl;

    Graph example = readGraph("resources/graph6.json");
    
    updateOverallIntersectionCount(example);




    for(auto count: example.getEdges())
    {
        std::cout << "Edge from ";
        printLocation(example.getNode(count.first.getSource_id()));
        std::cout << " to ";
        printLocation(example.getNode(count.first.getTarget_id()));
        std::cout << " intersects a total of " << count.second << " times." << std::endl;
    }

    
    Edge localPlanarEdge = example.getHighestIntersection();
    std::cout << "MAXIMUM INTERSECTING EDGE WITH " << example.getEdges()[localPlanarEdge] << " INTERSECTNG EDGES";
    printLocation(example.getNode(localPlanarEdge.getSource_id()));
    std::cout << " to ";
    printLocation(example.getNode(localPlanarEdge.getTarget_id()));



    writeGraph("output.json", example);



}
