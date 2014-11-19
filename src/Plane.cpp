#include "Plane.h"

// Plan passant par trois points
Plane::Plane(const Point& p1, const Point& p2, const Point& p3)
{
    Point normal = (p2 - p1) ^ (p3 - p1);
    double diff = normal * p1;

    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = -diff;
}
