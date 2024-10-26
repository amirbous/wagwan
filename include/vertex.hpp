#ifndef VERTEX_HPP
#define VERTEX_HPP

class Vertex{
    private:
        int id;
        int x;
        int y;

    public:
        // constructors
        Vertex (int id = -1, int x = -1, int y = -1) {
            this->id = id;
            this->x = x;
            this->y = y;
        }

        //getters and setters
        int getId()
        {
            return this->id;
        }

        int getX()
        {
            return this->x;
        }

        int getY()
        {
            return this->y;
        }
};


#endif