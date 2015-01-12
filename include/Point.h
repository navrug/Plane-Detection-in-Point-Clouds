#ifndef POINT_H
#define POINT_H

#include "Vec3.h"
#include <memory>

// A point is in a plane or not.
class Point : public Vec3d
{
public:
    inline Point(double x, double y, double z) :
        Vec3d(x, y, z) {}
    inline Point(const Vec3& position) :
        Vec3d(position) {}
/*
    inline bool equals(const std::shared_ptr<Point>& p)
        {return x == p->x && y == p->y && z == p->z;}
//*/
    //bool inPlane;
};

typedef std::shared_ptr<Point> SharedPoint;

#endif
