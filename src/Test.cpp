#include "Test.h"

// Create a base in random directions.
std::array<Vec3d, 3> Test::createBase(std::default_random_engine& generator)
{
    // Create two random unit vectors (uniformly distributed on sphere).
    std::normal_distribution<> dist;
    Vec3d u(dist(generator), dist(generator), dist(generator));
    u = u.normalized();

    Vec3d v(dist(generator), dist(generator), dist(generator));
    v = v.normalized();

    // (u, v, w) is orthonormal.
    Vec3d w = (u ^ v).normalized();
    v = (w ^ u).normalized();

    return std::array<Vec3d, 3>{u, v, w};
}

// Canonical base.
std::array<Vec3d, 3> Test::canonicalBase()
{
    return std::array<Vec3d, 3>{Vec3d(1, 0, 0), Vec3d(0, 1, 0), Vec3d(0, 0, 1)};
}

// Create artificial point clouds.
PointCloud Test::createTwoPlanes(std::default_random_engine& generator)
{
    std::array<Vec3d, 3> base = createBase(generator);

    PointCloud cloud;
    for (int x = -100 ; x <= 100 ; ++x)
        for (int y = -100 ; y <= 100 ; ++y)
            for (int z = -1 ; z <= 1 ; z += 2)
                cloud.addPoint(std::make_shared<Point>(base[0] * x + base[1] * y + base[2] * z * 3), RGB(0, 0, 0));
    cloud.boundingBox();
    return cloud;
}

PointCloud Test::createParallelPlanes(int size, unsigned int count, std::default_random_engine& generator)
{
    std::array<Vec3d, 3> base = createBase(generator);

    PointCloud cloud;
    for (int x = -size ; x <= size ; ++x)
        for (int y = -size ; y <= size ; ++y)
            for (int z = 0 ; z < count ; ++z)
                cloud.addPoint(std::make_shared<Point>(base[0] * x + base[1] * y + base[2] * z), RGB(0, 0, 0));
    cloud.boundingBox();
    return cloud;
}

PointCloud Test::createRandomPlanes(int size, unsigned int count, std::default_random_engine& generator)
{
    PointCloud cloud;
    for (unsigned int i = 0 ; i < count ; ++i)
    {
        std::array<Vec3d, 3> base = createBase(generator);

        // Plane (u, v).
        for (int x = -size ; x <= size ; ++x)
            for (int y = -size ; y <= size ; ++y)
                cloud.addPoint(std::make_shared<Point>(base[0] * x + base[1] * y), RGB(0, 0, 0));
    }
    cloud.boundingBox();
    return cloud;
}

PointCloud Test::createCube(int size, std::default_random_engine& generator)
{
    std::array<Vec3d, 3> base = createBase(generator);
    //std::array<Vec3d, 3> base = canonicalBase();

    PointCloud cloud;
    for (int x = -size ; x <= size ; ++x)
    {
        for (int y = -size ; y <= size ; ++y)
        {
            cloud.addPoint(std::make_shared<Point>(base[0]* x + base[1]* y + base[2] * size), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(base[0]* x + base[1]* y - base[2] * size), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(base[1]* x + base[2]* y + base[0] * size), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(base[1]* x + base[2]* y - base[0] * size), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(base[2]* x + base[0]* y + base[1] * size), RGB(0, 0, 0));
            cloud.addPoint(std::make_shared<Point>(base[2]* x + base[0]* y - base[1] * size), RGB(0, 0, 0));
        }
    }
    cloud.boundingBox();
    return cloud;
}

// Test least squares regression.
void Test::testLeastSquares()
{
    std::vector<SharedPoint> pts;
    for (int x = -100 ; x <= 100 ; ++x)
        for (int y = -100 ; y <= 100 ; ++y)
            for (int z = -1 ; z <= 1 ; z += 2)
                pts.push_back(std::make_shared<Point>(x, y, z));
    Plane plan(pts);
    std::cout << "best fit : " << plan << std::endl;
}
