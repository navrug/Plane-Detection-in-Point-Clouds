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
        Point* pointAt(int i) { return &points.at(i); }
        int size() const { return points.size(); }
        Vec3 getMiddle() { return middle; }
        Vec3 getHalfDimension() { return halfDimension; }
        virtual ~PointCloud();
    protected:
    private:
        Vec3 middle;
        Vec3 halfDimension;
        std::vector<Point> points;
};

#endif // POINTCLOUD_H
