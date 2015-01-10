#include "PointCloud.h"
#include "Ransac.h"
#include "Octree.h"

#include <iostream>
#include <random>
#include <math.h>
#include <time.h>

#include "Matrix.h"
#include "PlaneSet.h"

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

int main()
{

    std::string file = "Cloud.xyz";
//    std::string file = "scan148.3d";
    PointCloud cloud(file);
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

    //                  (depthThreshold, epsilon, numStartPoints, numPoints, generateur, planes, colors, dCos, dL, pts)

    clock_t begin = clock();

    octree.detectPlanes(100, 0.1, 10, 30, 10, random, planes, cloud.getColors(), std::cos(3.1415/180 * /*Angle in degrees: */ 5), 1);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Running time: " << elapsed_secs << " seconds." << std::endl;

    cloud.toPly("detect.ply", true);
    cloud.toPly("inplane.ply", false);

    return 0;
}
