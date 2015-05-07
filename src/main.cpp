#include "PointCloud.h"
#include "Octree.h"
#include "Test.h"

#include <chrono>
#include <fstream>

void runTest(PointCloud& cloud, const std::string& name, std::default_random_engine& random)
{
    std::cout << "****************************************" << std::endl;
    std::cout << "Testing : " << name << std::endl;
    std::cout << "****************************************" << std::endl;
    Octree octree(cloud, 30);
    std::cout << "Octree loaded !" << std::endl;

    std::vector<SharedPlane> planes;
    auto begin = std::chrono::steady_clock::now();

    // (depthThreshold, epsilon, numStartPoints, numPoints, steps, countRatio, generateur, planes, colors, dCos)
    octree.detectPlanes(100, 0.05, 10, 30, 10, 0.005, random, planes, cloud.colors(), std::cos(3.1415/180 * /*Angle in degrees: */ 15));

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_secs = end - begin;

    std::cout << std::endl << planes.size() << " planes :" << std::endl;
    std::sort(planes.begin(), planes.end(), [](const SharedPlane& a, const SharedPlane& b){return a->getCount() < b->getCount();});

    std::ofstream out((name + ".planes").c_str());
    for (unsigned int i = 0 ; i < planes.size() ; ++i)
    {
        std::cout << *planes[i] << std::endl;
        out << *planes[planes.size() - i - 1] << std::endl;
    }
    out.close();

    std::cout << std::endl << "Running time: " << elapsed_secs.count() << " seconds." << std::endl;

    std::cout << "Saving..." << std::endl;
    cloud.toPly(name + ".ply", true);
    std::cout << "****************************************" << std::endl;
    std::cout << "End of test : " << name << std::endl;
    std::cout << "****************************************" << std::endl;
}

int main()
{
    std::default_random_engine random;

    {
    PointCloud cloud = Test::createTwoPlanes(random);
    runTest(cloud, "detect_parallel_planes", random);
    }

    {
    PointCloud cloud = Test::createCube(20, random);
    cloud.merge(Test::createCube(50, random));
    cloud.merge(Test::createCube(80, random));
    runTest(cloud, "detect_3cubes", random);
    }

    {
    PointCloud cloud = Test::createRandomPlanes(50, 10, random);
    runTest(cloud, "detect_random_planes", random);
    }

    {
    PointCloud cloud;
    cloud.loadPly("Cloud.xyz");
    runTest(cloud, "detect_buildings", random);
    }

    return 0;
}
