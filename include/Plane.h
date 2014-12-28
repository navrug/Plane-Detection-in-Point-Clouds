#ifndef PLANE_H
#define PLANE_H

#include "Point.h"
#include <vector>
#include <iostream>
#include "RGB.h"

class Plane
{
public:
    Plane(double a_, double b_, double c_, double d_) : a(a_), b(b_), c(c_), d(d_) {}
    // Plan passant par trois points
    Plane(const Point& p1, const Point& p2, const Point& p3);
    // Plan passant par n points par minimisation des moindres carres
    Plane(const std::vector<SharedPoint>& pts);

    // Carre de la distance entre le point et le plan
    double distance(SharedPoint p);

    // Si le plan est bien defini (vecteur normal non nul)
    bool isValid() {return a != 0 || b != 0 || c != 0;}

    // Moindres carres
    void ajuste(const std::vector<SharedPoint>& pts);

    // Liste des points de ce plan
    void setPoints(const std::vector<SharedPoint>& pts);

    // Change la couleur des points du plan
    void setColor(RGB* color);

    // Décide si deux plans peuvent être fusionnés
    // dTheta est donné en radians.
    bool mergeableWith(const Plane& p, double dTheta) const;

    // Inclut le plan p dans le plan objet, le plan p est vidé.
    void merge(Plane& p);

private:
    //Coordinates corresponding to the plane equation ax+by+cz+d=0
    double a, b, c, d;
    std::vector<SharedPoint> points;
    //Attributes for plane merging
    Vec3 center; // Currently the barycenter, but it would be better to have the circumcenter if we could find an efficient computation method
    double radius;
    RGB* rgb;

    friend std::ostream& operator<<(std::ostream& os, const Plane& p);
};

#endif // PLANE_H
