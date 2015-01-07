#ifndef PLANESET_H
#define PLANESET_H

#include "Plane.h"


class PlaneSet
{
    public:
        PlaneSet();
        virtual ~PlaneSet();
        void addPlane(Plane& plane) { planes.push_back(plane); }
        void makeMerges(double dCos, double dL, UnionFind<SharedPoint, RGB>& colors);
    protected:
    private:
        std::vector<Plane> planes;
};

#endif // PLANESET_H
