#include "PointCloud.h"
#include "Ransac.h"
#include "Octree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <chrono>

#include "Matrix.h"
#include "PlaneSet.h"
#include "Test.h"

/*
template<size_t N>
void printArray(const std::array<double, N>& a)
{
    std::cout << "(";
    for (unsigned int i = 0 ; i < N ; ++i)
    {
        if (i)
            std::cout << ", ";
        std::cout << a[i];
    }
    std::cout << ")" << std::endl;
}
//*/

int main()
{
    Test::testSymmetricPoints();
    //return 0;

    PointCloud cloud;
    /*
    std::string file = "Cloud.xyz";
    cloud.loadPly(file);
    //*/
    /*
    std::string file = "../data/test.3d";
    cloud.load3D(file);
    //*/
    cloud = Test::symmetricCloud();
    /*
    std::string file = "scan148.3d";
    cloud.load3D(file);
    //*/
    std::cout << "Cloud loaded !" << std::endl;

    Octree octree(cloud);
    std::cout << "Octree loaded !" << std::endl;

    std::default_random_engine random;
    random.seed(333);
    /*
    Plane plane = ransac(cloud.getPoints(), 1, 3, 10, 100, random);
    std::cout << "Plan : " << plane << std::endl;
    plane.setColor(RGB(255, 0, 0));
    //*/
    std::vector<SharedPlane> planes;

    auto begin = std::chrono::steady_clock::now();

    // (depthThreshold, epsilon, numStartPoints, numPoints, generateur, planes, colors, dCos, dL, pts)
    octree.detectPlanes(100, 0.1, 10, 30, 10, random, planes, cloud.getColors(), std::cos(3.1415/180 * /*Angle in degrees: */ 5), 1);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_secs = end - begin;
    std::cout << "Running time: " << elapsed_secs.count() << " seconds." << std::endl;

    std::cout << std::endl << planes.size() << " planes :" << std::endl;
    std::sort(planes.begin(), planes.end(), [](const SharedPlane& a, const SharedPlane& b){return a->getCount() < b->getCount();});
    for (auto&& p : planes)
        std::cout << *p << std::endl;

    std::cout << "Saving..." << std::endl;
    cloud.toPly("detect.ply", true);
    cloud.toPly("inplane.ply", false);

    return 0;
}
