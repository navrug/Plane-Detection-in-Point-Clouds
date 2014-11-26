#ifndef RANSAC_H
#define RANSAC_H

#include "Plane.h"
#include <vector>
#include <random>

Plane ransac(const std::vector<SharedPoint>& points, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generateur);

#endif

