#include "PointCloud.h"
#include "Ransac.h"
#include "Octree.h"

#include <iostream>
#include <random>

#include "Matrix.h"
#include "PlaneSet.h"
#include "ColorRef.h"

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
    /*
    Teste le calcul de noyaux de matrices
    {
        std::array<std::array<double, 2>, 3> test = {{1, 2, 2, 4, 3, 6}};
        auto k = Matrix<3, 2>::getKernel(test);
        printArray(k);
    }
    {
        std::array<std::array<double, 3>, 2> test = {{1, 2, 2, 4, 3, 6}};
        auto k = Matrix<2, 3>::getKernel(test);
        printArray(k);
    }
    {
        std::array<std::array<double, 3>, 2> test = {{0, 2, 2, 0, 3, 6}};
        auto k = Matrix<2, 3>::getKernel(test);
        printArray(k);
    }
    {
        std::array<std::array<double, 2>, 3> test = {{1, 2, 3, 4, 3, 6}};
        auto k = Matrix<3, 2>::getKernel(test);
        printArray(k);
    }
    {
        std::array<std::array<double, 3>, 2> test = {{0, 2, 2, 1, 3, 6}};
        auto k = Matrix<2, 3>::getKernel(test);
        printArray(k);
    }

    return 0;
    //*/

//    std::string file = "Cloud.xyz";
//    //std::string file = "extract.xyz";
//    PointCloud cloud(file);
//    std::cout << "Cloud loaded !" << std::endl;
//
//    Octree octree(cloud);
//    std::cout << "Octree loaded !" << std::endl;
//
//    std::default_random_engine random;
//    random.seed(333);
//    /*
//    Plane plane = ransac(cloud.getPoints(), 1, 3, 10, 100, random);
//    std::cout << "Plan : " << plane << std::endl;
//    plane.setColor(RGB(255, 0, 0));
//    //*/
//    PlaneSet planes;
//    octree.ransac(5000, 1, 10, 30, 10, random, planes);
//
////    planes.display();
//    planes.makeMerges(0.05, 10);
//
//    cloud.toPly("detect3.ply");

    ColorRef* a = new ColorRef();
    ColorRef* b = new ColorRef();
    ColorRef* c = new ColorRef();
    ColorRef* d = new ColorRef();
    a->setNewRef(b);
    c->setNewRef(d);
    b->setNewRef(d);
    delete b;

    a->getRoot();

    return 0;
}
