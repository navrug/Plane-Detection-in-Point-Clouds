#ifndef PLANE_H
#define PLANE_H
#include <vector>

#include "Point.h"


class Plane
{
    public:
        Plane() {}
        virtual ~Plane() {}
        
        // Plan passant par trois points
        Plane(const Point& p1, const Point& p2, const Point& p3);
        
        // Moindres carres
        void ajuste(const std::vector<Point>& pts);
        
    protected:
    private:
        //Coordinates corresponding to the plane equation ax+by+cz+d=0
        double a, b, c, d;
        std::vector<Point> points;
};

#endif // PLANE_H
