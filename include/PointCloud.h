#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <string>
#include <list>
#include "Point.h"


class PointCloud
{
    public:
        PointCloud();
        PointCloud(char* filename);
        void addPoint(Point p) { points.push_back(p); }
        virtual ~PointCloud();
    protected:
    private:
        std::list<Point> points;
};

#endif // POINTCLOUD_H
