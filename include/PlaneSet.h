#ifndef PLANESET_H
#define PLANESET_H

#include "Plane.h"


class PlaneSet
{
    public:
        PlaneSet();
        virtual ~PlaneSet();
        void addPlane(Plane& plane) { planes.push_back(plane); }
        void makeMerges(double dTheta);
    protected:
    private:
        std::vector<Plane> planes;
};

#endif // PLANESET_H
