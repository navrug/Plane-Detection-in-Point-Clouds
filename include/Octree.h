#ifndef Octree_H
#define Octree_H

#include <vector>
#include "PointCloud.h"

class Octree {

    static unsigned int maxdepth;

public:
    Octree(const Vec3& origin, const Vec3& halfDimension);
    Octree(const PointCloud& cloud);

    // Determine which octant of the tree would contain 'point'
    int findOctant(SharedPoint p) const;

    bool isLeafNode() const;

    void insert(SharedPoint p, unsigned int depth);


    // This is a really simple routine for querying the tree for points
    // within a bounding box defined by min/max points (bmin, bmax)
    // All results are pushed into 'results'
    void getPointsInsideBox(const Vec3& bmin, const Vec3& bmax, std::vector<SharedPoint>& results);

private:
    // Physical position/size. This implicitly defines the bounding
    // box of this node
    Vec3 origin;         //! The physical center of this node
    Vec3 halfDimension;  //! Half the width/height/depth of this node

    // The tree has up to eight children and can additionally store
    // a point, though in many applications only, the leaves will store data.
    std::shared_ptr<Octree> children[8]; //! Pointers to child octants
    SharedPoint point;   //! Data point to be stored at a node
    unsigned int count; // number of points inside

    /*
            Children follow a predictable pattern to make accesses simple.
            Here, - means less than 'origin' in that dimension, + means greater than.
            child:	0 1 2 3 4 5 6 7
            x:      - - - - + + + +
            y:      - - + + - - + +
            z:      - + - + - + - +
     */

};

#endif
