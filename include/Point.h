#ifndef POINT_H
#define POINT_H

#include "Vec3.h"
#include "RGB.h"
#include <memory>

class Point : public Vec3
{
public:
	Point(double x, double y, double z, unsigned char r, unsigned char g, unsigned char b) : Vec3(x, y, z), rgb(r,g,b) {}
	Point(const Vec3& position) : Vec3(position) {}
	bool equals(const std::shared_ptr<Point>& p) {return x == p->x && y == p->y && z == p->z;}
	
    RGB rgb;
};

typedef std::shared_ptr<Point> SharedPoint;

#endif
