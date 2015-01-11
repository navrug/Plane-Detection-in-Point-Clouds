#ifndef TEST_H
#define TEST_H

#include "PointCloud.h"

class Test
{
public:
    static PointCloud createTwoPlanes();
    static PointCloud createParallelPlanes(int size, unsigned int count);
    static void testSymmetricPoints();
};

#endif // TEST_H
