#include <map>
#include <vector>
#include <algorithm>

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/graphalg/PlanarSeparatorModule.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/BoyerMyrvold.h>

#include "../include/structs.hpp"
#include "../include/intersectGraph.hpp"

#include "../include/RTree.h"
#include "../include/structs.hpp"

bool rectanglesIntersect(const double* rec_min, const double* rec_max, Rectangle* rectangle) {
    return !(rec_min[0] > rectangle->xmax || rec_max[0] < rectangle->xmin || 
             rec_min[1] > rectangle->ymax || rec_max[1] < rectangle->ymin);
}


std::map<int, ogdf::edge, std::greater<int>> check_edge_intersections(const ogdf::Graph& G, 
                                                                    const ogdf::GraphAttributes& GA, 
                                                                    RTree<Rectangle*, double, 2>& rtree)
                                                                    {
    std::map<int, ogdf::edge, std::greater<int>> intersections; // Map to store intersections and corresponding edges

    // Iterate over the edges of the graph
    for (const auto& e : G.edges) {
        auto source = e->source();
        auto target = e->target();

        // Define the rectangle min and max coordinates for the current edge
        double rec_min[2] = {
            (std::min(GA.x(target), GA.x(source))),
            (std::min(GA.y(target), GA.y(source)))
        };
        double rec_max[2] = {
            (std::max(GA.x(target), GA.x(source))),
            (std::max(GA.y(target), GA.y(source)))
        };

        int intersection_count = 0; // To count the number of intersections for the current edge

        // Define a callback to be invoked for each rectangle found in the RTree search
        auto callback = [&rec_min, &rec_max, &intersection_count](Rectangle* rect) -> bool {

            // Check if the current edge's rectangle intersects with the found rectangle
            if (rectanglesIntersect(rec_min, rec_max, rect)) {
            double recother_min[2] = {
                rect->xmin, rect->ymin
            };
            double recother_max[2] = {
                rect->xmax, rect->ymax
            };
                if (edgesIntersect(rec_min, rec_max, recother_min, recother_max))
                intersection_count++; // Increment intersection count
            }
            return true; // Continue searching for more intersections
        };

        // Perform the search for intersecting rectangles within the bounding box of the edge
        rtree.Search(rec_min, rec_max, callback);

        // Store the result in the map: edge and the number of intersections
        intersections[intersection_count] = e;
    }

    return intersections; // Return the map of intersections
}

std::pair<RTree<Rectangle*, double, 2>, std::map<ogdf::edge, Rectangle*>> createTree(const ogdf::Graph &G, const ogdf::GraphAttributes &GA) {

    std::map<ogdf::edge, Rectangle*> edgeRectangle;
    RTree<Rectangle*, double, 2> rtree; // Use double for both the bounding box and data type


     for (const auto& e : G.edges) {
        auto source = e->source();
        auto target = e->target();

        // Correctly define the rectangle min and max coordinates as doubles
        double rec_min[2] = {
            (std::min(GA.x(target), GA.x(source))),
            (std::min(GA.y(target), GA.y(source)))
        };
        double rec_max[2] = {
            (std::max(GA.x(target), GA.x(source))),
            (std::max(GA.y(target), GA.y(source)))
        };

        // Create the Rectangle object
        Rectangle* rec = new Rectangle( rec_min[0], rec_min[1], rec_max[0], rec_max[1] );
        edgeRectangle[e] = rec;

        // Insert the bounding box and the Rectangle as the data in the RTree
        rtree.Insert(rec_min, rec_max, rec);  
    }
    return {rtree, edgeRectangle};

}

void UpdateTree (RTree<Rectangle*, double, 2>& rtree, ogdf::edge e,
                    std::map<ogdf::edge, Rectangle*> & edgeRectangle,
                    const double* recnew_min, const double* recnew_max) {
    Rectangle* worstRectangle = edgeRectangle[e];
    double recold_min[2] = {
        worstRectangle->xmin, worstRectangle->ymin
    };
    double recold_max[2] = {
        worstRectangle->xmax, worstRectangle->ymax
    };
    rtree.Remove(recold_min, recold_max, worstRectangle);
    Rectangle* newRect = new Rectangle(recnew_min[0], recnew_min[1], recnew_max[0], recnew_max[1]);
    
    rtree.Insert(recnew_min, recnew_max, newRect);
    edgeRectangle[e] = newRect;
}