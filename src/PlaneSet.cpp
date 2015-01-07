#include "PlaneSet.h"

#include "Plane.h"
#include <iostream>


PlaneSet::PlaneSet()
{
    //ctor
}

PlaneSet::~PlaneSet()
{
    //dtor
}

//For each plane, try to merge it with neughbouring planes
//We could optimize this by using an octree for the planes, or by inserting the planes in the octree.
void PlaneSet::makeMerges(double dTheta, double dL) {
    for (int i(0); i<planes.size()-1; ++i) {
        std::cout << *planes[i] << std::endl;
        for (int j(i+1); j<planes.size(); ++j) {
            if (planes[i]->mergeableWith(*planes[j], dTheta, dL)) {
                planes[j]->merge(*planes[i]);
                break;
            }
        }
    }
}


void PlaneSet::display() {
//    for (int i(0); i<planes.size()-1; ++i)
    int i = 0;
    for (Plane* pl : planes) {
            std::cout << "Plane " << i++ << ": " << *pl << std::endl;
    }
}
