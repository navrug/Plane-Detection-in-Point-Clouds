#include "Plane.h"

#include "Matrix.h"
#include "Vec3.h"
#include <math.h>

std::ostream& operator<<(std::ostream& os, const Plane& p)
{
    return os << "{" << p.a << "x + " << p.b << "y + " << p.c << "z + " << p.d << " : " << p.count << " points, radius = " << p.radius << ", thickness = " << p.thickness << "}";
}

// Plan invalide
Plane::Plane()
{
    this->init();
    a = b = c = d = 0;
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

// Accepte un point dans le plan
bool Plane::accept(SharedPoint p)
{
    return this->distance(p) <= 3 * thickness;
}

// Ajoute un point au plan (sans recalculer l'equation)
void Plane::addPoint(SharedPoint p, UnionFind<SharedPoint, RGB>& colors)
{
    this->addPoint(*p);
    if (point)
        colors.merge(p, point);
    else
        point = p;
    p->inPlane = true;
}


// Initialise la matrice M
void Plane::init()
{
    count = 0;

    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 4 ; ++j)
            m[i][j] = 0;

    center = Vec3();
    radius = 0;
}

// Ajoute un point au plan (sans recalculer l'equation)
void Plane::addPoint(const Point& p)
{
    ++count;

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
    for (double& x : eq)
        x /= norm;

    a = eq[0];
    b = eq[1];
    c = eq[2];
    d = eq[3];

    center = Vec3(m[0][3], m[1][3], m[2][3]) / count;
    // Var(X) = Moy(X^2) - Moy(X)^2
    Vec3 stddev = (Vec3(m[0][0], m[1][1], m[2][2]) / count) - center.product(center);
    radius = std::sqrt(stddev.x + stddev.y + stddev.z);

    // Var(X dot U) = Moy((X dot U)^2) - Moy(X dot U)^2
    double meansq = 0;
    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 3 ; ++j)
            meansq += m[i][j] * eq[i] * eq[j];
    meansq /= count;

    double mean = 0;
    for (unsigned int i = 0 ; i < 3 ; ++i)
        mean += m[i][3] * eq[i];
    mean /= count;

    thickness = std::sqrt(meansq - mean * mean);
}

// Lie un plan à ses points, calcule au passage le barycentre et le rayon du nuage de points.
void Plane::setPoints(const std::vector<SharedPoint>& pts)
{
    //count = pts.size();
    if (!pts.empty())
        point = pts[0];

    this->init();
    for (auto&& p : pts)
        this->addPoint(*p);
    this->computeEquation();
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

    //*
    // Calcul de l'écart angulaire
    double cos = getCos(*this, p);
    if (cos < dCos)
        return false;
    //*/

    //*
    // Test éloignement des barycentres selon la normale
    if (this->distanceAlong(center, p.center) > thickness && p.distanceAlong(center, p.center) > p.thickness)
        return false;
    //*/

    /*
    // Test écart angulaire avec le nouveau plan
    Plane tempPlane;
    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 4 ; ++j)
            tempPlane.m[i][j] = m[i][j] + p.m[i][j];
    tempPlane.count = count + p.count;
    tempPlane.computeEquation();

    if (getCos(tempPlane, *this)<dCos || getCos(tempPlane, p)<dCos)
        return false;
    if (tempPlane.thickness > 3 * std::max(thickness, p.thickness))
        return false;
    //*/

    return true;
}


// Inclut le plan p dans le plan objet, le plan p est vidé.
void Plane::merge(Plane& p, UnionFind<SharedPoint, RGB>& colors) {
    std::cout << "Merge :" << std::endl;
    std::cout << *this << std::endl;
    std::cout << p << std::endl;

    count += p.count;
    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 4 ; ++j)
            m[i][j] += p.m[i][j];

    colors.merge(point, p.point);
    this->computeEquation();

    std::cout << *this << std::endl;

    p = Plane();
}

double Plane::distanceAlong(Vec3 u, Vec3 v) const {
    return std::abs(((v.x-u.x)*a + (v.y-u.y)*b + (v.z-u.z)*c)/sqrt(a*a + b*b + c*c));
}

double Plane::getCos(const Plane& p, const Plane& q) {
    return std::fabs((q.a*p.a + q.b*p.b + q.c*p.c) / sqrt((q.a*q.a + q.b*q.b + q.c*q.c)*(p.a*p.a + p.b*p.b + p.c*p.c)));
}
