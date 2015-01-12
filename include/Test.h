#ifndef TEST_H
#define TEST_H

#include "PointCloud.h"
#include <random>
#include <array>

// Class to generate tests.
class Test
{
public:
    // Create artificial point clouds, in random directions.
    static PointCloud createTwoPlanes(std::default_random_engine& generator);
    static PointCloud createParallelPlanes(int size, unsigned int count, std::default_random_engine& generator);
    static PointCloud createRandomPlanes(int size, unsigned int count, std::default_random_engine& generator);
    static PointCloud createCube(int size, std::default_random_engine& generator);
    // Test least squares regression.
    static void testLeastSquares();

private:
    // Create a base in random directions.
    static std::array<Vec3d, 3> createBase(std::default_random_engine& generator);
    // Canonical base.
    static std::array<Vec3d, 3> canonicalBase();
};

#endif // TEST_H
