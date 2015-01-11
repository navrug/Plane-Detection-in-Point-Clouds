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

PointCloud Test::createRandomPlanes(int size, unsigned int count, std::default_random_engine& generateur)
{
    PointCloud cloud;
    for (unsigned int i = 0 ; i < count ; ++i)
    {
        // Crée deux vecteurs unitaires aléatoires (uniformément distribués sur la sphère).
        std::normal_distribution<> distribution;
        double a = distribution(generateur);
        double b = distribution(generateur);
        double c = distribution(generateur);
        Vec3 u(a, b, c);
        u = u.normalized();

        double d = distribution(generateur);
        double e = distribution(generateur);
        double f = distribution(generateur);
        Vec3 v(d, e, f);
        v = v.normalized();

        // Plan Vect(u, v).
        for (int x = -size ; x <= size ; ++x)
            for (int y = -size ; y <= size ; ++y)
                cloud.addPoint(std::make_shared<Point>(u * x + v * y), RGB(0, 0, 0));
    }
    cloud.finishLoad();
    return cloud;
}

PointCloud Test::createCube(int size)
{
    PointCloud cloud;
    for (int x = -size ; x <= size ; ++x)
    {
        for (int y = -size ; y <= size ; ++y)
        {
            cloud.addPoint(std::make_shared<Point>(x, y, size), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(x, y, -size), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(x, size, y), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(x, -size, y), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(size, x, y), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(-size, x, y), RGB(0, 0, 0));
        }
    }
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
