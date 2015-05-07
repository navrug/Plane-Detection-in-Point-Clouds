#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <string>
#include <vector>
#include <memory>
#include "Plane.h"
#include "UnionFind.h"

// Set of points.
class PointCloud
{
    friend class Test;

public:
    // Empty cloud.
    PointCloud();
    // Load from file.
    void loadPly(const std::string& filename);
    void load3D(const std::string& filename);
    // Save to file.
    bool toPly(const std::string& filename, bool showPlanes) const;

    // Merge two point clouds.
    void merge(const PointCloud& other);

    // Accessors.
    inline Vec3d center() const
        {return mCenter;}
    inline Vec3d halfDimension() const
        {return mHalfDimension;}
    inline const std::vector<SharedPoint>& points() const
        {return mPoints;}
    inline UnionFindPlanes& colors()
        {return mColors;}
    
private:
    // Add point.
    void addPoint(SharedPoint p, RGB color);
    // Compute bounding box.
    void boundingBox();

    // Points and bounding box.
    Vec3d mCenter;
    Vec3d mHalfDimension;
    Vec3d min;
    Vec3d max;
    std::vector<SharedPoint> mPoints;
    UnionFindPlanes mColors;
};

#endif // POINTCLOUD_H
