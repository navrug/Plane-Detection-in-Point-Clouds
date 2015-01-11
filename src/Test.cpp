#include "Test.h"

PointCloud Test::symmetricCloud()
{
    PointCloud cloud;
    for (int x = -100 ; x <= 100 ; ++x)
        for (int y = -100 ; y <= 100 ; ++y)
            for (int z = -1 ; z <= 1 ; z += 2)
                cloud.addPoint(std::make_shared<Point>(x, y, z), RGB(x + 100, y + 100, 100*z + 100));
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
