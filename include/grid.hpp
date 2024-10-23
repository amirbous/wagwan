#ifndef GRID_HPP
#define GRID_HPP


class Grid {
    private:
        int height;
        int width;
        // have to check how to allocate, whether store only occupied points, or big malloc (not that practical)
        // dont't quite understand the role of the structure points mentionned in the input file

    public:
        Grid(){};
        Grid(int height, int width) {
            this->height = height;
            this->width = width;
        }

};

#endif