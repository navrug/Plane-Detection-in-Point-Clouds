#include "PlaneSet.h"

#include "Plane.h"



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
void PlaneSet::makeMerges() {

}
