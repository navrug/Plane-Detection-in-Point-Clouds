#ifndef RANSAC_H
#define RANSAC_H

#include "Plane.h"
#include <vector>
#include <random>
#include "PlaneSet.h"

class Ransac
{
public:
    static SharedPlane ransac(std::vector<SharedPoint>& points, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generateur, UnionFind<SharedPoint, RGB>& colors);
};

#endif

