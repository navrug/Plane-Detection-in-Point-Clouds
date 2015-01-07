#include "Plane.h"

#include "Matrix.h"
#include "Vec3.h"
#include <math.h>

std::ostream& operator<<(std::ostream& os, const Plane& p)
{
    return os << "{" << p.a << "x + " << p.b << "y + " << p.c << "z + " << p.d << " : " << p.count << " points}";
}

// Plan invalide
Plane::Plane()
{
    this->init();
    a = b = c = d = 0;
    count = 0;
}

// Plan passant par n points par minimisation des moindres carres
Plane::Plane(const std::vector<SharedPoint>& pts)
{
    this->setPoints(pts);
}


// Carre de la distance entre le point et le plan
double Plane::distance(SharedPoint p)
{
    double diff = a*p->x + b*p->y + c*p->z + d;
    double norm = a*a + b*b + c*c;
    return diff * diff / norm;
}


// Initialise la matrice M
void Plane::init()
{
    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 4 ; ++j)
            m[i][j] = 0;
}

// Ajoute un point a la matrice M
void Plane::addPoint(const Point& p)
{
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

// Calcule l'equation par minimisation des moindres carres.
void Plane::computeEquation()
{
    // L'equation du plan est un vecteur du noyau de M
    std::array<double, 4> eq = Matrix<3, 4>::getKernel(m);
    double norm = sqrt(eq[0]*eq[0] + eq[1]*eq[1] + eq[2]*eq[2]);
    a = eq[0] / norm;
    b = eq[1] / norm;
    c = eq[2] / norm;
    d = eq[3] / norm;
}

// Calcule la sphere englobant les points du plan.
void Plane::computeSphere(const std::vector<SharedPoint>& points)
{
    center = Vec3();
    for (auto&& point : points)
        center += *point;
    center /= points.size();

    radius = 0;
    for (auto&& point : points)
    {
        double d = center.distance(*point);
        if (d > radius)
            radius = d;
    }
}

// Lie un plan à ses points, calcule au passage le barycentre et le rayon du nuage de points.
void Plane::setPoints(const std::vector<SharedPoint>& pts)
{
    count = pts.size();
    if (!pts.empty())
        point = pts[0];

    this->init();
    for (auto&& p : pts)
        this->addPoint(*p);
    this->computeEquation();
    this->computeSphere(pts);
}

// Change la couleur des points du plan
void Plane::setColor(RGB color, UnionFind<SharedPoint, RGB>& colors)
{
    colors.set(point, color);
}


// Décide si deux plans peuvent être fusionnés
// dTheta est donné en radians.
bool Plane::mergeableWith(const Plane& p, double dCos, double dL) const {
    // Plan vide
    if (!(count && p.count))
        return false;

    // Les spheres circonscrites sintersectent-elles:
    if (center.distance(p.center) > (radius + p.radius))
        return false;
    // Calcul de l'écart angulaire
    double cos = (a*p.a + b*p.b + c*p.c) / sqrt((a*a + b*b + c*c)*(p.a*p.a + p.b*p.b + p.c*p.c));
    if (cos < dCos)
        return false;
    // Comparaison des composantes affines
    //if (std::abs(d - p.d) > dL)
      //  return false;
    //Ajout d'un test point par point ?

    return true;
}

// Inclut le plan p dans le plan objet, le plan p est vidé.
void Plane::merge(Plane& p, UnionFind<SharedPoint, RGB>& colors) {
    std::cout << "Merge :" << std::endl;
    std::cout << *this << std::endl;
    std::cout << p << std::endl;
    std::cout << count << " + " << p.count << " = " << count + p.count << std::endl;

    radius = center.distance(p.center) + std::max(radius, p.radius);
    center = (center * count + p.center * p.count) / (count + p.count);
    colors.merge(point, p.point);

    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 4 ; ++j)
            m[i][j] += p.m[i][j];
    count += p.count;

    this->computeEquation();

    p = Plane();
}
