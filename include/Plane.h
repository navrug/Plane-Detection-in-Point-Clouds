#ifndef PLANE_H
#define PLANE_H

#include "Point.h"
#include <vector>
#include <iostream>
#include "RGB.h"
#include "ColorRef.h"

class Plane
{
public:
    Plane(double a_, double b_, double c_, double d_) : a(a_), b(b_), c(c_), d(d_) { colorRef = new ColorRef(); }
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
    void setColor(RGB color);

    // Décide si deux plans peuvent être fusionnés
    // dTheta est donné en radians.
    bool mergeableWith(const Plane& p, double dTheta, double dL) const;

    // Inclut le plan p dans le plan objet, le plan p est vidé.
    void merge(Plane& p);

    // Structure union-find pour la couleur des points via les plans de référence
//    ColorRef* getColorRef() {
//        colorRef = colorRef->getRoot();
//        return colorRef;
//    }
    void setNewRef(ColorRef* newRef) {
        colorRef->setNewRef(newRef);
    }

private:
    //Coordinates corresponding to the plane equation ax+by+cz+d=0
    double a, b, c, d;
    std::vector<SharedPoint> points;
    //Attributes for plane merging
    Vec3 center; // Currently the barycenter, but it would be better to have the circumcenter if we could find an efficient computation method
    double radius;
    RGB rgb;
    ColorRef* colorRef;
    //Matrix for plane equation computations
    std::array<std::array<double, 4>, 3> m;

    friend std::ostream& operator<<(std::ostream& os, const Plane& p);
};

#endif // PLANE_H
