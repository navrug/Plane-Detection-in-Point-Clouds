#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <string>
#include <vector>
#include <memory>
#include "Plane.h"


class PointCloud
{
public:
    PointCloud(const std::string& filename);
    
    bool toPly(const std::string& filename);
    void addPoint(Point p) { points.push_back(std::make_shared<Point>(p)); }
    SharedPoint pointAt(int i) const { return points.at(i); }
    int size() const { return points.size(); }
    Vec3 getMiddle() const { return middle; }
    Vec3 getHalfDimension() const { return halfDimension; }
    const std::vector<SharedPoint>& getPoints() {return points;}
    
private:
    Vec3 middle;
    Vec3 halfDimension;
    std::vector<SharedPoint> points;
};

#endif // POINTCLOUD_H
