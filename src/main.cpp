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
    Plane plane = ransac(cloud.getPoints(), 1, 3, 10, 100, random);
    std::cout << "Plan : " << plane << std::endl;

    plane.setColor(RGB(255, 0, 0));
    cloud.toPly("detect.ply");

    //Octree octree(cloud);
    //std::cout << "Octree loaded !" << std::endl;

    return 0;
}
