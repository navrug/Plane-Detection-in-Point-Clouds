#ifndef POLYGON_H
#define POLYGON_H
#include <list>


class Polygon
{
    public:
        Polygon() {}
        virtual ~Polygon() {}
    protected:
    private:
        Plane plane;
        std::list<Point> points;
        Point barycenter;
        double radius;
};

#endif // POLYGON_H
