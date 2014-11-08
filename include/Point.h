#ifndef POINT_H
#define POINT_H
#include "Vec3.h"


class Point
{
	Vec3 position;
public:
	Point() { }
	Point(double x, double y, double z) { position = Vec3(x, y, z); }
	Point(const Vec3& position) : position(position) { }
	inline const Vec3& getPosition() const { return position; }
	inline void setPosition(const Vec3& p) { position = p; }
};

#endif
