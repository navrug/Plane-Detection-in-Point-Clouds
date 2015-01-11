#ifndef TEST_H
#define TEST_H

#include "PointCloud.h"
#include <random>

class Test
{
public:
    static PointCloud createTwoPlanes();
    static PointCloud createParallelPlanes(int size, unsigned int count);
    static PointCloud createRandomPlanes(int size, unsigned int count, std::default_random_engine& generateur);
    static PointCloud createCube(int size);
    static void testSymmetricPoints();
};

#endif // TEST_H
