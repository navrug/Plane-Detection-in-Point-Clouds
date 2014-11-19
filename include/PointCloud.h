#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <string>
#include <vector>
#include "Point.h"


class PointCloud
{
    public:
        PointCloud();
        PointCloud(const std::string& filename);
        void addPoint(Point p) { points.push_back(p); }
        virtual ~PointCloud();
    protected:
    private:
        std::vector<Point> points;
};

#endif // POINTCLOUD_H
