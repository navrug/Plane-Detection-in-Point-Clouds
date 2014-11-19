#include "PointCloud.h"
#include "Ransac.h"

#include <iostream>
#include <random>

int main()
{
    PointCloud cloud("Cloud.xyz");
    std::cout << "Cloud loaded !" << std::endl;

    std::default_random_engine random;
    Plane plane = ransac(cloud.points, 1, 3, 10, 100, random);
    std::cout << "Plan : " << plane << std::endl;

    return 0;
}
