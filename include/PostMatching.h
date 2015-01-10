#ifndef POSTMATCHING_H
#define POSTMATCHING_H

#include "PointCloud.h"
#include "PlaneSet.h"



static void postMatch(PointCloud& points, std::vector<SharedPlane>& planes, double lambda) {
    for (int i(0); i<points.size(); ++i) {
        SharedPoint p = points.pointAt(i);
        if (!p->inPlane) {
            for (SharedPlane plane : planes) {
                if (Plane::distanceAlong(*p, plane->getCenter(), *plane) < lambda*plane->getDeviation()) {
                    (points.getColors()).set(p, points.getColors().at(plane->getPoint()));
                    p->inPlane = true;
                }
            }
        }
    }
}


//        static void postMatch(PointCloud& points, const PlaneSet& planes, double lambda);


#endif // POSTMATCHING_H
