#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <string>
#include <vector>
#include <memory>
#include "Plane.h"
#include "UnionFind.h"


class PointCloud
{
    friend class Test;
public:
    PointCloud();
    void loadPly(const std::string& filename);
    void load3D(const std::string& filename);
    
    bool toPly(const std::string& filename, bool showPlanes);
    SharedPoint pointAt(int i) const { return points.at(i); }
    int size() const { return points.size(); }
    Vec3 getCenter() const { return center; }
    Vec3 getHalfDimension() const { return halfDimension; }
    const std::vector<SharedPoint>& getPoints() {return points;}
    UnionFind<SharedPoint, RGB>& getColors() {return colors;}
    
private:
    void addPoint(SharedPoint p, RGB color);
    void finishLoad();

    Vec3 center;
    Vec3 min;
    Vec3 max;
    Vec3 halfDimension;
    std::vector<SharedPoint> points;
    UnionFind<SharedPoint, RGB> colors;
};

#endif // POINTCLOUD_H
