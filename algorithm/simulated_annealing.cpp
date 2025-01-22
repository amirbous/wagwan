

#include <set>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/intersectGraph.hpp"
#include "../include/RTree.h"
#include "../include/structs.hpp"

#include <set>
#include <limits.h>

std::pair<int,int> generate_new_source(std::mt19937 &gen, double &temperature, int &source_x, int &source_y, int &width, int &height)
{
    int x,y;
    do
    {
        std::uniform_real_distribution<> angle_dist(0, 2 * M_PI);
        std::uniform_int_distribution<> length_dist(0, static_cast<int>(temperature * 50));

        double angle = angle_dist(gen);
        int length = length_dist(gen);

        double new_x = source_x + length * std::cos(angle);
        double new_y = source_y + length * std::sin(angle);

        x = static_cast<int>(std::round(new_x));
        y = static_cast<int>(std::round(new_y));
    }
    while (x < 0 || y < 0 || x >= width || y >= height );

    return {x,y};
}


void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node, int> &nodes_id, 
                        int max_iterations, RTree<Rectangle*, double, 2> & rtree, std::map<ogdf::edge, Rectangle*> & edgeRectangle, 
                            int width, int height, int cooling_technique, double initial_temperature, double cooling_rate)
{
    const static unsigned int seed = 42;
    std::mt19937 gen(seed);

    auto probability = [](double energy, double energy_new, double temperature) {
        return (energy_new < energy) ? 1.0 : (temperature == 0 ? 0 : std::exp(-(energy_new - energy) / temperature));
    };
    
    std::map<std::pair<int, int>, bool> check_node{};
    for (auto node : G.nodes)
    {
            check_node[{GA.x(node), GA.y(node)}] = true;
    }

    int iteration_count = 0;

    double energy = 0, energy_new = INT_MAX;

    int highestCount = INT_MAX;

    std::map<int, ogdf::edge,  std::greater<int>> intersection_edges{},
                                                local_intersection_edges{};


    std::vector<ogdf::node> source(10), target(10);
    std::vector<ogdf::edge> worst_edges(10);

    int source_x = 0, source_y = 0;


    double temperature;
    while (iteration_count < max_iterations && highestCount > 0) {
        if (cooling_technique == 0)
            temperature = 1.0 - static_cast<double>(iteration_count + 1) / max_iterations;
        else if (cooling_technique == 1)
            temperature = initial_temperature / log(static_cast<double>(iteration_count + 2));
        else temperature = initial_temperature * std::pow(cooling_rate, iteration_count);


        // extracting data about worst edge
        intersection_edges = check_edge_intersections(G, GA, rtree);
                

        // check if already no intersections are found :)
        if (intersection_edges.empty()) {
            std::cout << "REACHED 0 INTERSECTION WITHIN " << iteration_count << " ITERATIONS!" << std::endl;
            return;
        }


        auto  lookupEdge_it = intersection_edges.begin();
        auto highestCount = lookupEdge_it->first;
        std::cout << "In iteration number= " << iteration_count << ", Count=" << highestCount << std::endl;

        for(int n_sources=0; n_sources < 10; n_sources++)
        {
            if (lookupEdge_it != intersection_edges.end()) {
                ogdf::edge worst_edge = lookupEdge_it->second;
                worst_edges[n_sources] = (worst_edge);
                source[n_sources] = (worst_edge->source());
                target[n_sources] = (worst_edge->target());
                lookupEdge_it++;
            }
        }
        


        // start simulated annealing
        energy = highestCount;

        
        std::vector<std::pair<int, int>> new_sources_vector(20);
        for (int i = 0; i < 20; i++) {
            if (i >= new_sources_vector.size()) std::cout << "ta7che" << std::endl;
            new_sources_vector[i] = generate_new_source(gen, temperature, source_x, source_y, width, height);
        }

        std::vector<std::pair<int,int>> previous_sources(10);
        std::vector<std::pair<int,int>> new_sources(10);
        for(int edge_index=0; edge_index < source.size(); edge_index++)
        {

            bool check = false;

            // Usefull later for evaluating the energy, as only the incident edges will be changed locally,
            // However global evaluation in outer loop requires the full edge list from the graph
            std::vector<ogdf::edge> source_incident_edges = getIncidentEdges(G, GA, source[edge_index]);
            source_x = GA.x(source[edge_index]);
            source_y = GA.y(source[edge_index]);
            
            for(auto new_source: new_sources_vector)
            {
                if (check)
                    break;
                GA.x(source[edge_index]) = new_source.first;
                GA.y(source[edge_index]) = new_source.second;
                if (!check_node[new_source] && !check_node_on_anyEdge(G,GA,source[edge_index])
                        && !edgeIntersects_anyNode(G, GA, worst_edges[edge_index]) && !nodeEdges_intersect_anyNode(G, GA, source[edge_index]))
                {
                    double recNew_min[2] = {
                        (std::min(GA.x(target[edge_index]), GA.x(source[edge_index]))),
                        (std::min(GA.y(target[edge_index]), GA.y(source[edge_index])))
                    };
                    double recNew_max[2] = {
                        (std::max(GA.x(target[edge_index]), GA.x(source[edge_index]))),
                        (std::max(GA.y(target[edge_index]), GA.y(source[edge_index])))
                    };
                    UpdateTree(rtree, worst_edges[edge_index], edgeRectangle, recNew_min, recNew_max);
                    // again using the tree to evaluate intersections
                    auto newIntersection_edges = check_edge_intersections(G, GA, rtree);

                    energy_new = newIntersection_edges.begin()->first;

                    std::uniform_real_distribution<> distribution2(0.0, 1.0);
                    double random_probability = distribution2(gen);

                    if (probability(energy, energy_new, temperature) >= random_probability)
                    {
                        // we add the edge between the new source node and the target node
                        check_node[{source_x, source_y}] = false;
                        previous_sources[edge_index] = {source_x, source_y};
                        check_node[new_source] = true;
                        new_sources[edge_index] = new_source;
                        check = true;
                    }
                    else
                    {
                        // re-create the edge if the new configuration is not accepted
                        GA.x(source[edge_index]) = source_x;
                        GA.y(source[edge_index]) = source_y;
                        double recNew_min[2] = {
                            (std::min(GA.x(target[edge_index]), GA.x(source[edge_index]))),
                            (std::min(GA.y(target[edge_index]), GA.y(source[edge_index])))
                        };
                        double recNew_max[2] = {
                            (std::max(GA.x(target[edge_index]), GA.x(source[edge_index]))),
                            (std::max(GA.y(target[edge_index]), GA.y(source[edge_index])))
                        };
                        UpdateTree(rtree, worst_edges[edge_index], edgeRectangle, recNew_min, recNew_max);
                        }
                }
                else
                {
                    GA.x(source[edge_index]) = source_x;
                    GA.y(source[edge_index]) = source_y;
                }
            }
        }

        /*energy_new = check_edge_intersections(G, GA, rtree).begin()->first;

        std::uniform_real_distribution<> distribution2(0.0, 1.0);
        double random_probability = distribution2(gen);
        if (probability(energy, energy_new, temperature) >= random_probability)
        {
            std::cout << "MOVING 10 NODES IN ONE STEP" << std::endl;
        }
        else
        {
            for(int edge_index=0;edge_index<source.size();edge_index++)
            {
                check_node[new_sources[edge_index]] = false;
                check_node[previous_sources[edge_index]] = true;
                GA.x(source[edge_index]) = previous_sources[edge_index].first;
                GA.y(source[edge_index]) = previous_sources[edge_index].second;
                double recNew_min[2] = {
                    (std::min(GA.x(target[edge_index]), GA.x(source[edge_index]))),
                    (std::min(GA.y(target[edge_index]), GA.y(source[edge_index])))
                };
                double recNew_max[2] = {
                    (std::max(GA.x(target[edge_index]), GA.x(source[edge_index]))),
                    (std::max(GA.y(target[edge_index]), GA.y(source[edge_index])))
                };
                UpdateTree(rtree, worst_edges[edge_index], edgeRectangle, recNew_min, recNew_max);
            }
        }*/

        iteration_count ++;
    }
    std::cout << "MAXIMUM NUMBER OF ITERATIONS ACHIEVED" << std::endl;

}


/*


void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node, int> &nodes_id, 
                        int max_iterations, RTree<Rectangle*, double, 2> & rtree, std::map<ogdf::edge, Rectangle*> & edgeRectangle, 
                            int width, int height, int cooling_technique, double initial_temperature, double cooling_rate)
{
    const static unsigned int seed = 42;
    std::mt19937 gen(seed);

    auto probability = [](double energy, double energy_new, double temperature) {
        return (energy_new < energy) ? 1.0 : (temperature == 0 ? 0 : std::exp(-(energy_new - energy) / temperature));
    };
    
    std::map<std::pair<int, int>, bool> check_node{};
    for (auto node : G.nodes)
    {
            check_node[{GA.x(node), GA.y(node)}] = true;
    }

    int iteration_count = 0;
    double energy = 0, energy_new = INT_MAX;
    int highestCount = INT_MAX;
    std::map<int, ogdf::edge,  std::greater<int>> intersection_edges{},
<<<<<<< Updated upstream
                                                local_intersection_edges{};
    std::vector<ogdf::node> source{}, target{};
    std::vector<ogdf::edge> worst_edges;
=======
                                                newIntersection_edges{};
    ogdf::node source{},
                target{};
>>>>>>> Stashed changes
    int source_x = 0, source_y = 0;
    double temperature;
    while (iteration_count < max_iterations && highestCount > 0) {
       
        if (cooling_technique == 0)
            temperature = 1.0 - (iteration_count + 1) / max_iterations;
        else if (cooling_technique == 1)
            temperature = initial_temperature / log((iteration_count + 2));
        else temperature = initial_temperature * std::pow(cooling_rate, iteration_count);


        // extracting data about worst edge
        intersection_edges = check_edge_intersections(G, GA, rtree);
                

        // check if already no intersections are found :)
        if (intersection_edges.empty()) {
            std::cout << "REACHED 0 INTERSECTION WITHIN " << iteration_count << " ITERATIONS!" << std::endl;
            return;
        }

        //worst edge = first element of tree
        auto  lookupEdge_it = intersection_edges.begin();

        for(int n_sources=0; n_sources < 10; n_sources++)
        {
            if (lookupEdge_it != intersection_edges.end()) {
                ogdf::edge worst_edge = lookupEdge_it++->second;
                worst_edges.push_back(worst_edge);
                source.push_back(worst_edge->source());
                target.push_back(worst_edge->target());
            }
        }

<<<<<<< Updated upstream



        highestCount = calculate_specific_intersections(findIntersections(G,GA), worst_edges[0]);
=======
        ogdf::edge worst_edge = lookupEdge_it->second;
        std::cout << "In iteration number= " << iteration_count << ", Count=" << lookupEdge_it->first << std::endl;

        source = worst_edge->source();
        target = worst_edge->target();

        
        source_x = GA.x(source);
        source_y = GA.y(source);

        highestCount = lookupEdge_it->first;
>>>>>>> Stashed changes

        // start simulated annealing
        energy = highestCount;

        std::vector<std::pair<int, int>> new_sources_vector;
        while(new_sources_vector.size() < 20)
            new_sources_vector.push_back(generate_new_source(gen, temperature, source_x, source_y, width, height));

        std::vector<std::pair<int,int>> previous_sources(10);
        std::vector<std::pair<int,int>> new_sources(10);
        for(int edge_index=0; edge_index < source.size(); edge_index++)
        {
            bool check = false;

            // Usefull later for evaluating the energy, as only the incident edges will be changed locally,
            // However global evaluation in outer loop requires the full edge list from the graph
            std::vector<ogdf::edge> source_incident_edges = getIncidentEdges(G, GA, source[edge_index]);
            source_x = GA.x(source[edge_index]);
            source_y = GA.y(source[edge_index]);

            for(auto new_source: new_sources_vector)
            {
<<<<<<< Updated upstream
                if (check)
                    break;
                GA.x(source[edge_index]) = new_source.first;
                GA.y(source[edge_index]) = new_source.second;
                if (!check_node[new_source] && !check_node_on_anyEdge(G,GA,source[edge_index])
                        && !edgeIntersects_anyNode(G, GA, worst_edges[edge_index]) && !nodeEdges_intersect_anyNode(G, GA, source[edge_index]))
                {

                    // to reduce time for eavluating edge intersections, instead of evaluating whole edge, just the edge with the worst

                    std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G, GA, source_incident_edges);
=======
                double recNew_min[2] = {
                    (std::min(GA.x(target), GA.x(source))),
                    (std::min(GA.y(target), GA.y(source)))
                };
                double recNew_max[2] = {
                    (std::max(GA.x(target), GA.x(source))),
                    (std::max(GA.y(target), GA.y(source)))
                };
                UpdateTree(rtree, worst_edge, edgeRectangle, recNew_min, recNew_max);
                // again using the tree to evaluate intersections
                newIntersection_edges = check_edge_intersections(G, GA, rtree);
                if (newIntersection_edges.empty()) {
                    std::cout << "finished" << std::endl;
                    return;
                }
                energy_new = newIntersection_edges.begin()->first;
>>>>>>> Stashed changes

                    energy_new = calculate_singular_intersections(intersections).begin()->first;



                    std::uniform_real_distribution<> distribution2(0.0, 1.0);
                    double random_probability = distribution2(gen);

                    if (probability(energy, energy_new, temperature) >= random_probability)
                    {
                        // we add the edge between the new source node and the target node
                        check_node[{source_x, source_y}] = false;
                        previous_sources[edge_index] = {source_x, source_y};
                        check_node[new_source] = true;
                        new_sources[edge_index] = new_source;
                        check = true;
                    }
                    else
                    {
                        // re-create the edge if the new configuration is not accepted
                        GA.x(source[edge_index]) = source_x;
                        GA.y(source[edge_index]) = source_y;
                    }
                }
                else
                {
<<<<<<< Updated upstream
                    GA.x(source[edge_index]) = source_x;
                    GA.y(source[edge_index]) = source_y;
=======
                    // re-create the edge if the new configuration is not accepted
                    GA.x(source) = source_x;
                    GA.y(source) = source_y;
                    double recNew_min[2] = {
                        (std::min(GA.x(target), GA.x(source))),
                        (std::min(GA.y(target), GA.y(source)))
                    };
                    double recNew_max[2] = {
                        (std::max(GA.x(target), GA.x(source))),
                        (std::max(GA.y(target), GA.y(source)))
                    };
                    UpdateTree(rtree, worst_edge, edgeRectangle, recNew_min, recNew_max);

>>>>>>> Stashed changes
                }
            }
        }

        std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G, GA, worst_edges);

        energy_new = calculate_singular_intersections(intersections).begin()->first;

        std::uniform_real_distribution<> distribution2(0.0, 1.0);
        double random_probability = distribution2(gen);
        if (probability(energy, energy_new, temperature) >= random_probability)
        {
            std::cout << "MOVING 10 NODES IN ONE STEP" << std::endl;
        }
        else
        {
            for(int edge_index=0;edge_index<source.size();edge_index++)
            {
<<<<<<< Updated upstream
                check_node[new_sources[edge_index]] = false;
                check_node[previous_sources[edge_index]] = true;
                GA.x(source[edge_index]) = previous_sources[edge_index].first;
                GA.y(source[edge_index]) = previous_sources[edge_index].second;
=======
                GA.x(source) = source_x;
                GA.y(source) = source_y;
                
>>>>>>> Stashed changes
            }
        }

        iteration_count ++;
    }
    std::cout << "MAXIMUM NUMBER OF ITERATIONS ACHIEVED" << std::endl;

}

*/
