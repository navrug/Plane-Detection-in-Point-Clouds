#ifndef POINT_H
#define POINT_H

#include "Vec3.h"

class Point : public Vec3
{
public:
	Point(double x, double y, double z) : Vec3(x, y, z) {}
	Point(const Vec3& position) : Vec3(position) {}
};

#endif
