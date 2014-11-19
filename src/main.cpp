#include "PointCloud.h"
#include "Ransac.h"
#include "Octree.h"

#include <iostream>
#include <random>

int main()
{
    std::string file = "Cloud.xyz";
    //std::string file = "extract.xyz";
    PointCloud cloud(file);
    std::cout << "Cloud loaded !" << std::endl;

    std::default_random_engine random;
    Plane plane = ransac(cloud.points, 1, 3, 10, 100, random);
    std::cout << "Plan : " << plane << std::endl;

    Octree octree(cloud);
    cout << "Octree loaded !" << endl;

    return 0;
}
