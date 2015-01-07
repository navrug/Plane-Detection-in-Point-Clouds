#ifndef PLANE_H
#define PLANE_H

#include "Point.h"
#include <vector>
#include <iostream>
#include "RGB.h"
#include "UnionFind.h"

class Plane
{
public:
    //Plane(double a_, double b_, double c_, double d_) : a(a_), b(b_), c(c_), d(d_) {}
    Plane();
    // Plan passant par trois points
    //Plane(const Point& p1, const Point& p2, const Point& p3);
    // Plan passant par n points par minimisation des moindres carres
    Plane(const std::vector<SharedPoint>& pts);

    // Carre de la distance entre le point et le plan
    double distance(SharedPoint p);

    // Si le plan est bien defini (vecteur normal non nul)
    bool isValid() {return a != 0 || b != 0 || c != 0;}

    // Lie un plan à ses points, calcule au passage le barycentre et le rayon du nuage de points
    void setPoints(const std::vector<SharedPoint>& pts);

    // Change la couleur des points du plan
    void setColor(RGB color, UnionFind<SharedPoint, RGB>& colors);

    // Decide si deux plans peuvent être fusionnes
    // dTheta est donne en radians.
    bool mergeableWith(const Plane& p, double dCos, double dL) const;

    // Inclut le plan p dans le plan objet, le plan p est vide.
    void merge(Plane& p, UnionFind<SharedPoint, RGB>& colors);

private:
    // Initialise la matrice M
    void init();
    // Ajoute un point a la matrice M
    void addPoint(const Point& p);
    // Calcule l'equation par minimisation des moindres carres.
    void computeEquation();
    // Calcule la sphere englobant les points du plan.
    void computeSphere(const std::vector<SharedPoint>& points);

    //Coordinates corresponding to the plane equation ax+by+cz+d=0
    double a, b, c, d;

    // Matrix to compute the optimal equation given the points
    std::array<std::array<double, 4>, 3> m;
    // One point related to the plane
    SharedPoint point;
    // Number of points
    unsigned int count;

    //Attributes for plane merging
    Vec3 center; // Currently the barycenter, but it would be better to have the circumcenter if we could find an efficient computation method
    double radius;

    friend std::ostream& operator<<(std::ostream& os, const Plane& p);
};

typedef std::shared_ptr<Plane> SharedPlane;

#endif // PLANE_H
