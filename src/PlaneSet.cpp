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
void PlaneSet::makeMerges(double dTheta) {
    for (int i(0); i<planes.size()-1; ++i) {
        std::cout << planes[i] << std::endl;
        for (int j(i+1); j<planes.size(); ++j) {
            if (planes[i].mergeableWith(planes[j], dTheta)) {
                planes[j].merge(planes[i]);
                break;
            }
        }
    }
}
