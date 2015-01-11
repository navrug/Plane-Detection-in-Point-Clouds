#include "Test.h"

PointCloud Test::createTwoPlanes()
{
    PointCloud cloud;
    for (int x = -100 ; x <= 100 ; ++x)
        for (int y = -100 ; y <= 100 ; ++y)
            for (int z = -1 ; z <= 1 ; z += 2)
                cloud.addPoint(std::make_shared<Point>(x, y, z), RGB(0, 0, 0));
    cloud.finishLoad();
    return cloud;
}

PointCloud Test::createParallelPlanes(int size, unsigned int count)
{
    PointCloud cloud;
    for (int x = -size ; x <= size ; ++x)
        for (int y = -size ; y <= size ; ++y)
            for (int z = 0 ; z < count ; ++z)
                cloud.addPoint(std::make_shared<Point>(x, y, z), RGB(0, 0, 0));
    cloud.finishLoad();
    return cloud;
}

void Test::testSymmetricPoints()
{
    std::vector<SharedPoint> pts;
    for (int x = -100 ; x <= 100 ; ++x)
        for (int y = -100 ; y <= 100 ; ++y)
            for (int z = -1 ; z <= 1 ; z += 2)
                pts.push_back(std::make_shared<Point>(x, y, z));
    Plane plan(pts);
    std::cout << "best fit : " << plan << std::endl;
}
