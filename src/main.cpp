#include "PointCloud.h"
#include "Octree.h"
#include "Test.h"

#include <chrono>

PointCloud getCloud(std::default_random_engine& random)
{
    PointCloud cloud;
    //*
    std::string file = "Cloud.xyz";
    cloud.loadPly(file);
    //*/
    /*
    std::string file = "../data/test.3d";
    cloud.load3D(file);
    //*/
    //cloud = Test::createTwoPlanes(random);
    //cloud = Test::createParallelPlanes(100, 20, random);
    /*
    cloud = Test::createCube(20, random);
    cloud.merge(Test::createCube(50, random));
    //*/
    //cloud = Test::createRandomPlanes(50, 10, random);
    /*
    std::string file = "scan148.3d";
    cloud.load3D(file);
    //*/

    return cloud;
}

int main()
{
    std::default_random_engine random;

    PointCloud cloud = getCloud(random);
    std::cout << "Cloud loaded !" << std::endl;

    Octree octree(cloud, 30);
    std::cout << "Octree loaded !" << std::endl;

    std::vector<SharedPlane> planes;
    auto begin = std::chrono::steady_clock::now();

    // (depthThreshold, epsilon, numStartPoints, numPoints, steps, countRatio, generateur, planes, colors, dCos)
    octree.detectPlanes(100, 0.05, 10, 30, 10, 0.005, random, planes, cloud.colors(), std::cos(3.1415/180 * /*Angle in degrees: */ 15));

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_secs = end - begin;
    std::cout << "Running time: " << elapsed_secs.count() << " seconds." << std::endl;

    std::cout << std::endl << planes.size() << " planes :" << std::endl;
    std::sort(planes.begin(), planes.end(), [](const SharedPlane& a, const SharedPlane& b){return a->getCount() < b->getCount();});
    for (auto&& p : planes)
        std::cout << *p << std::endl;

    std::cout << "Saving..." << std::endl;
    cloud.toPly("detect.ply", true);
    //cloud.toPly("inplane.ply", false);

    return 0;
}
