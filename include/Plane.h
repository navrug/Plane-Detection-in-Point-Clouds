#ifndef PLANE_H
#define PLANE_H

#include "Point.h"
#include <vector>
#include <iostream>


class Plane
{
public:
    Plane(double a_, double b_, double c_, double d_) : a(a_), b(b_), c(c_), d(d_) {}
    // Plan passant par trois points
    Plane(const Point& p1, const Point& p2, const Point& p3);
    
    // Carre de la distance entre le point et le plan
    double distance(const Point& p);
    
    // Si le plan est bien defini (vecteur normal non nul)
    bool isValid() {return a != 0 || b != 0 || c != 0;}
    
    // Moindres carres
    void ajuste(const std::vector<Point>& pts);
    
    // Liste des points de ce plan
    void setPoints(const std::vector<Point>& pts) {points = pts;}
    
private:
    //Coordinates corresponding to the plane equation ax+by+cz+d=0
    double a, b, c, d;
    std::vector<Point> points;
    
    friend std::ostream& operator<<(std::ostream& os, const Plane& p);
};

#endif // PLANE_H
