#ifndef POINT_H
#define POINT_H

#include "Vec3.h"
#include "RGB.h"
#include <memory>

class Point : public Vec3
{
public:
    inline Point(double x, double y, double z) :
        Vec3(x, y, z), inPlane(false) {}
    inline Point(const Vec3& position) :
        Vec3(position), inPlane(false) {}

    inline bool equals(const std::shared_ptr<Point>& p)
        {return x == p->x && y == p->y && z == p->z;}

    bool inPlane;
};

typedef std::shared_ptr<Point> SharedPoint;

#endif
