#include "Plane.h"

std::ostream& operator<<(std::ostream& os, const Plane& p)
{
    return os << "{" << p.a << "x + " << p.b << "y + " << p.c << "z + " << p.d << " : " << p.points.size() << " points}";
}

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

// Carre de la distance entre le point et le plan
double Plane::distance(const Point& p)
{
    double diff = a*p.x + b*p.y + c*p.z + d;
    double norm = a*a + b*b + c*c;
    return diff * diff / norm;
}
