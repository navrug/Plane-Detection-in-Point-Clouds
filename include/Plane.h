#ifndef PLANE_H
#define PLANE_H
#include <list>


class Plane
{
    public:
        Plane() {}
        virtual ~Plane() {}
    protected:
    private:
        //Coordinates corresponding to the plane equation ax+by+cz+d=0
        double a, b, c, d;
        std::list<Point> points;
};

#endif // PLANE_H
