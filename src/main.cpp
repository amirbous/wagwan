#include <iostream>

#include "../include/graph.hpp"
#include "../include/IO.hpp"
#include "../include/utils.hpp"

void printLocation(Node node) 
{
    std::cout <<" (" << node.getX() << ", " << node.getY() << ") ";
}
int main() {


	std::cout << "Welcome to Wagwan" << std::endl;
	std::cout <<"This code runs fine" << std::endl;

    Graph example = readGraph("resources/ex3_k0_xr0.json");

    std::vector<std::pair<Edge,Edge>> intersections = findIntersections(example);
    std::map<Edge,int> intersection_count;
    for(std::pair<Edge,Edge> edges : intersections)
    {
        intersection_count[edges.first]++;
        intersection_count[edges.second]++;
    }

    for(auto count: intersection_count)
    {
        std::cout << "Edge from ";
        printLocation(example.getNode(count.first.getSource_id()));
        std::cout << " to ";
        printLocation(example.getNode(count.first.getTarget_id()));
        std::cout << " intersects a total of " << count.second << " times." << std::endl;
    }

    writeGraph("output.json", example);



}
