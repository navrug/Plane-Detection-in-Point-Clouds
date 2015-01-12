#ifndef RANSAC_H
#define RANSAC_H

#include "Plane.h"
#include <vector>
#include <random>

class Ransac
{
public:
    // Find a plane with RANSAC algorithm.
    static SharedPlane ransac(std::vector<SharedPoint>& points, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generator, UnionFindPlanes& colors);

private:
    // Match points close to the plane.
    static void matchPoints(const std::vector<SharedPoint>& points, Plane& plane, double epsilon, std::vector<SharedPoint>& pts_int, std::vector<SharedPoint>& pts_out);
};

#endif
