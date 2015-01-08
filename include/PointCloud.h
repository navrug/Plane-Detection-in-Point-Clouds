#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <string>
#include <vector>
#include <memory>
#include "Plane.h"
#include "UnionFind.h"


class PointCloud
{
public:
    PointCloud(const std::string& filename);
    
    bool toPly(const std::string& filename, bool showPlanes);
    void addPoint(SharedPoint p, RGB color);
    SharedPoint pointAt(int i) const { return points.at(i); }
    int size() const { return points.size(); }
    Vec3 getMiddle() const { return middle; }
    Vec3 getHalfDimension() const { return halfDimension; }
    const std::vector<SharedPoint>& getPoints() {return points;}
    UnionFind<SharedPoint, RGB>& getColors() {return colors;}
    
private:
    Vec3 middle;
    Vec3 halfDimension;
    std::vector<SharedPoint> points;
    UnionFind<SharedPoint, RGB> colors;
};

#endif // POINTCLOUD_H
