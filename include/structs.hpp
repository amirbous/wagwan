#ifndef STRUCTS_HPP
#define STRUCTS_HPP


#include "RTree.h"

class Rectangle {

public:
    double xmin, ymin; // Minimum corner (bottom-left)
    double xmax, ymax; // Maximum corner (top-right)
    Rectangle(double xmin, double ymin, double xmax, double ymax) {
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
    };
    // Optionally, define helper functions for the RTree library's requirements.
    bool intersects(const Rectangle& other) const {
        return xmax >= other.xmin && xmin <= other.xmax &&
               ymax >= other.ymin && ymin <= other.ymax;
    }

    bool contains(const Rectangle& other) const {
        return xmin <= other.xmin && xmax >= other.xmax &&
               ymin <= other.ymin && ymax >= other.ymax;
    }

    bool rectanglesIntersect(double min1[2], double max1[2], double min2[2], double max2[2]) {
    // Check if the rectangles intersect or touch
    return !(min1[0] > max2[0] || max1[0] < min2[0] || min1[1] > max2[1] || max1[1] < min2[1]);
}
};

std::pair<RTree<Rectangle*, double, 2>, std::map<ogdf::edge, Rectangle*>> createTree(const ogdf::Graph &G, const ogdf::GraphAttributes &GA);

bool rectanglesIntersect(const double* rec_min, const double* rec_max, Rectangle* rectangle);
std::map<int, ogdf::edge, std::greater<int>> check_edge_intersections(const ogdf::Graph& G, 
                                                                    const ogdf::GraphAttributes& GA, 
                                                                    RTree<Rectangle*, double, 2>& rtree);

void UpdateTree (RTree<Rectangle*, double, 2>& rtree, ogdf::edge e,
                    std::map<ogdf::edge, Rectangle*> & edgeRectangle,
                    const double* recnew_min, const double* recnew_max);

#endif
