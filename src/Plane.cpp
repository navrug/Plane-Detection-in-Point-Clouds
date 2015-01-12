#include "Plane.h"

#include "Vec3.h"
#include <cmath>

// Print the plane.
std::ostream& operator<<(std::ostream& os, const Plane& p)
{
    return os << "{" << p.normal[0] << "x + " << p.normal[1] << "y + " << p.normal[2] << "z + " << p.d << " : " << p.count << " points, radius = " << p.radius << ", thickness = " << p.thickness << "}";
}


// Invalid plane.
Plane::Plane() :
    m(3, 3, CV_64FC1)
{
    this->init();
    d = 0;
}

// Plane that best fit the points with least squares minimization.
Plane::Plane(const std::vector<SharedPoint>& pts) :
    m(3, 3, CV_64FC1)
{
    this->setPoints(pts);
}


// Distance between point and plane.
double Plane::distance(SharedPoint p)
{
    return std::abs((normal * *p) + d);
}

// Square of distance between point and plane.
double Plane::squareDistance(SharedPoint p)
{
    double diff = (normal * *p) + d;
    return diff * diff;
}

// Whether the point is close to the plane.
bool Plane::accept(SharedPoint p)
{
    return (center.distance(*p) < 3 * radius) && (this->distance(p) < 2 * thickness);
}


// Add a point to the plane (without recomputing equation).
void Plane::addPoint(SharedPoint p, UnionFindPlanes& colors)
{
    this->addPoint(*p);
    if (point)
        colors.merge(p, point);
    else
        point = p;
}


// Fit the plane to the points.
void Plane::setPoints(const std::vector<SharedPoint>& pts)
{
    if (!pts.empty())
        point = pts[0];

    this->init();
    for (auto&& p : pts)
        this->addPoint(*p);
    this->computeEquation();
}

// Change the color of the plane.
void Plane::setColor(RGB color, UnionFindPlanes& colors)
{
    colors.set(point, std::make_pair(color, true));
}

// Reset all points to their initial color.
void Plane::destroy(UnionFindPlanes& colors)
{
    colors.reset(point);
}


// Merge plane p into this.
void Plane::merge(Plane& p, UnionFindPlanes& colors)
{
    count += p.count;
    m += p.m;
    sum += p.sum;
    this->computeEquation();

    colors.merge(point, p.point);
    p = Plane();
}

// Decides whether p is mergeable with this.
bool Plane::mergeableWith(const Plane& p, double dCos) const {
    // Empty plane.
    if (!(count && p.count))
        return false;

    // Angular difference between normals.
    if (this->getCos(p) < dCos)
        return false;
    /*
    if (center.distance(p.center) > 2 * (radius + p.radius))
        return false;
    //*/
    // Distance along normals.
    if (this->distanceAlong(center, p.center) > thickness && p.distanceAlong(center, p.center) > p.thickness)
        return false;

    // Angular difference with new plane.
    Plane tempPlane;
    tempPlane.m = m + p.m;
    tempPlane.sum = sum + p.sum;
    tempPlane.count = count + p.count;
    tempPlane.computeEquation();

    if (this->getCos(tempPlane) < dCos || p.getCos(tempPlane) < dCos)
        return false;
    if (tempPlane.radius > radius + p.radius)
        return false;
    if (tempPlane.thickness > thickness + p.thickness)
        return false;

    /*
    std::cout << *this << std::endl;
    std::cout << p << std::endl;
    std::cout << "cos : " << this->getCos(p) << std::endl;
    //*/

    return true;
}


// Initialize plane attributes.
void Plane::init()
{
    count = 0;

    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 3 ; ++j)
            m.at<double>(i, j) = 0;
    sum = Vec3d();

    center = Vec3d();
    radius = 0;
}

// Add a point to the plane (without recomputing equation).
void Plane::addPoint(const Point& p)
{
    ++count;

    m.at<double>(0, 0) += p.x * p.x;
    m.at<double>(0, 1) += p.x * p.y;
    m.at<double>(0, 2) += p.x * p.z;

    m.at<double>(1, 0) += p.y * p.x;
    m.at<double>(1, 1) += p.y * p.y;
    m.at<double>(1, 2) += p.y * p.z;

    m.at<double>(2, 0) += p.z * p.x;
    m.at<double>(2, 1) += p.z * p.y;
    m.at<double>(2, 2) += p.z * p.z;

    sum += p;
}

// Best fit of the plane using least squares.
void Plane::leastSquares()
{
    cv::Mat mat(3, 3, CV_64FC1);
    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 3 ; ++j)
            mat.at<double>(i, j) = m.at<double>(i, j) - sum[i] * sum[j] / count;

    cv::Mat eigenvals;
    cv::Mat eigenvects;
    cv::eigen(mat, eigenvals, eigenvects);

    normal[0] = eigenvects.at<double>(2, 0);
    normal[1] = eigenvects.at<double>(2, 1);
    normal[2] = eigenvects.at<double>(2, 2);
    normal.normalize();
    d = - (normal * sum) / count;
}

// Compute equation and attributes of the plane (radius, thickness)
void Plane::computeEquation()
{
    this->leastSquares();

    center = sum / count;

    // Var(X) = Mean(X^2) - Mean(X)^2
    Vec3d stddev = (Vec3d(m.at<double>(0, 0), m.at<double>(1, 1), m.at<double>(2, 2)) / count)
            - center.cmul(center);
    radius = std::sqrt(stddev.x + stddev.y + stddev.z);

    // Var(X dot U) = Mean((X dot U)^2) - Mean(X dot U)^2
    double meansq = 0;
    for (unsigned int i = 0 ; i < 3 ; ++i)
        for (unsigned int j = 0 ; j < 3 ; ++j)
            meansq += m.at<double>(i, j) * normal[i] * normal[j];
    meansq /= count;

    double mean = d; // (normal * sum) / count;

    // Usually due to rounding errors when points are perfectly aligned.
    if (meansq - mean * mean < 0)
        thickness = 0;
    else
        thickness = std::sqrt(meansq - mean * mean);

    // Useful for perfectly aligned points where thickness = 0;
    if (thickness < radius / 1000)
        thickness = radius / 1000;
}

// Distance betwen u and v along the normal.
double Plane::distanceAlong(Vec3d u, Vec3d v) const
{
    return std::abs((v - u) * normal);
}

// Angular difference between normals.
double Plane::getCos(const Plane& p) const
{
    return std::abs(normal * p.normal);
}
