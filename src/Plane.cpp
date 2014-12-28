#include "Plane.h"

#include "Matrix.h"
#include "Vec3.h"
#include <math.h>

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

// Plan passant par n points par minimisation des moindres carres
Plane::Plane(const std::vector<SharedPoint>& pts)
{
    std::array<std::array<double, 4>, 3> m;

    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 4 ; ++j)
            m[i][j] = 0;

    for (unsigned int n = 0 ; n < pts.size() ; ++n)
    {
        const Point& p = *pts[n];

        m[0][0] += p.x * p.x;
        m[0][1] += p.x * p.y;
        m[0][2] += p.x * p.z;
        m[0][3] += p.x;

        m[1][0] += p.y * p.x;
        m[1][1] += p.y * p.y;
        m[1][2] += p.y * p.z;
        m[1][3] += p.y;

        m[2][0] += p.z * p.x;
        m[2][1] += p.z * p.y;
        m[2][2] += p.z * p.z;
        m[2][3] += p.z;
    }

    // L'equation du plan est un vecteur du noyau de m
    std::array<double, 4> eq = Matrix<3, 4>::getKernel(m);
    double norm = sqrt(eq[0]*eq[0] + eq[1]*eq[1] + eq[2]*eq[2]);
    a = eq[0]/norm;
    b = eq[1]/norm;
    c = eq[2]/norm;
    d = eq[3]/norm;
}

// Carre de la distance entre le point et le plan
double Plane::distance(SharedPoint p)
{
    double diff = a*p->x + b*p->y + c*p->z + d;
    double norm = a*a + b*b + c*c;
    return diff * diff / norm;
}

// Lie un plan à ses points, calcule au passage le barycentre et le rayon du nuage de points.
void Plane::setPoints(const std::vector<SharedPoint>& pts) {
    points = pts;
    for (SharedPoint point : points) {
        center += static_cast<Vec3>(*point); //! Comment faire un dynamic cast sur un sharedptr ?
    }
    center /= points.size();
    radius = 0;
    double d;
    for (SharedPoint point : points) {
        d = center.distance(static_cast<Vec3>(*point));
        radius = d > radius ? d : radius;
    }
}

// Change la couleur des points du plan
void Plane::setColor(RGB* color)
{
    rgb = color;
    for (SharedPoint p : points)
        p->rgb = color;
}


// Décide si deux plans peuvent être fusionnés
// dTheta est donné en radians.
bool Plane::mergeableWith(const Plane& p, double dTheta) const {
    //Les spheres circonscrites s’intersectent-elles:
    if (center.distance(p.center) > radius + p.radius)
        return false;
    //Calcul de l’écart angulaire
    double cos = (a*p.a + b*p.b + c*p.c) / sqrt((a*a + b*b + c*c)*(p.a*p.a + p.b*p.b + p.c*p.c));
    if ((1-cos)*(1-cos) > dTheta) // We use the approximation cos x = 1 - x*x
        return false;
    //Ajout d’un test point par point ?

    return true;
}

// Inclut le plan p dans le plan objet, le plan p est vidé.
void Plane::merge(Plane& p) {
    std::cout << *this << std::endl;
    std::cout << points.size() << " + " << p.points.size() << " = " <<  points.size() + p. points.size() << std::endl;
    radius = center.distance(p.center) + std::max(radius, p.radius);
    center = (points.size() + p.points.size()) * (center/points.size() + p.center/p.points.size());
    points.reserve(points.size() + p.points.size());
    points.insert(points.end(), p.points.begin(), p.points.end());
    p.points.clear();

    // On recalcule l’equation du plan, on change au passage la couleur des points
    std::array<std::array<double, 4>, 3> m;

    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 4 ; ++j)
            m[i][j] = 0;

    for (unsigned int n = 0 ; n < points.size() ; ++n)
    {
        Point& p = *points[n];
        p.rgb = rgb;

        m[0][0] += p.x * p.x;
        m[0][1] += p.x * p.y;
        m[0][2] += p.x * p.z;
        m[0][3] += p.x;

        m[1][0] += p.y * p.x;
        m[1][1] += p.y * p.y;
        m[1][2] += p.y * p.z;
        m[1][3] += p.y;

        m[2][0] += p.z * p.x;
        m[2][1] += p.z * p.y;
        m[2][2] += p.z * p.z;
        m[2][3] += p.z;
    }

    // L'equation du plan est un vecteur du noyau de m
    std::array<double, 4> eq = Matrix<3, 4>::getKernel(m);
    double norm = sqrt(eq[0]*eq[0] + eq[1]*eq[1] + eq[2]*eq[2]);
    a = eq[0]/norm;
    b = eq[1]/norm;
    c = eq[2]/norm;
    d = eq[3]/norm;
    std::cout << "Merged in " << *this << std::endl << std::endl;
}


