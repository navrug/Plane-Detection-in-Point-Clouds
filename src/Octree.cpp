#include "Octree.h"

#include "Ransac.h"
#include "PlaneSet.h"

unsigned int Octree::maxdepth = 30;


Octree::Octree(const Vec3& origin, const Vec3& halfDimension) :
    origin(origin), halfDimension(halfDimension), count(0)
{
}

Octree::Octree(const PointCloud& cloud) :
    origin(cloud.getMiddle()), halfDimension(cloud.getHalfDimension()), count(0)
{
    for (int i = 0 ; i < cloud.size() ; ++i)
    {
        SharedPoint p = cloud.pointAt(i);
        insert(p, 0);
    }
}


void Octree::getPoints(std::vector<std::shared_ptr<Point> >& pts) const
{
    if (isLeafNode())
    {
        if (point.get() != nullptr)
            pts.push_back(point);
    }
    else
        for (auto&& child : children)
            child->getPoints(pts);
}

void Octree::detectPlanes(int depthThreshold, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generateur, std::vector<SharedPlane>& planes, UnionFind<SharedPoint, RGB>& colors, double dCos, double dL) const
{
    // Recursion
    if (count > depthThreshold)
    {
        std::vector<SharedPlane> set;
        for (auto&& child : children)
            if (child.get() != nullptr)
                child->detectPlanes(depthThreshold, epsilon, numStartPoints, numPoints, steps, generateur, set, colors, dCos, dL);

        for (unsigned int i = 0 ; i < set.size() ; ++i)
        {
            for (unsigned int j = 0 ; j < i ; ++j)
            {
                if (set[i] && set[j] && set[i]->mergeableWith(*set[j], dCos, dL))
                {
                    set[i]->merge(*set[j], colors);
                    set[j].reset();
                }
            }
        }

        for (unsigned int i = 0 ; i < set.size() ; ++i)
            if (set[i])
                planes.push_back(set[i]);
    }
    // Do ransac
    else
    {
        std::vector<std::shared_ptr<Point> > pts;
        this->getPoints(pts);

        SharedPlane plane = Ransac::ransac(pts, epsilon, numStartPoints, numPoints, steps, generateur, colors);
        if (!plane)
            return;

        planes.push_back(plane);

        std::uniform_int_distribution<int> distribution(0, 255);
        auto random = std::bind(distribution, generateur);
        int r = random();
        int g = random();
        int b = random();
        plane->setColor(RGB(r, g, b), colors);
    }
}


// Determine which octant of the tree would contain 'point'
int Octree::findOctant(SharedPoint p) const
{
    int oct = 0;
    if (p->x >= origin.x) oct |= 4;
    if (p->y >= origin.y) oct |= 2;
    if (p->z >= origin.z) oct |= 1;
    return oct;
}

bool Octree::isLeafNode() const
{
    return children[0].get() == nullptr;
}

void Octree::insert(SharedPoint p, unsigned int depth)
{
    if (depth >= maxdepth)
    {
        std::cout << "Profondeur maximale atteinte. Point non ajouté : [" << p->x << ", " << p->y << ", " << p->z << "]" << std::endl;
        return;
    }

    //std::cout << "insert in {(" << origin.x << ", " << origin.y << ", " << origin.z << "), (" << halfDimension.x << ", " << halfDimension.y << ", " << halfDimension.z << ")}" << std::endl;

    // If this node doesn't have a data point yet assigned
    // and it is a leaf, then we're done!
    if (!isLeafNode())
    {
        // We are at an interior node. Insert recursively into the
        // appropriate child octant

        //std::cout << ">>node : findOctant(p) : " << findOctant(p) << std::endl;

        children[findOctant(p)]->insert(p, depth + 1);
        ++count;
    }
    else
    {
        if (point.get() == nullptr)
        {
            point = p;
            ++count;
        }
        else
        {
            // We're at a leaf, but there's already something here
            // We will split this node so that it has 8 child octants
            // and then insert the old data that was here, along with
            // this new data point

            /*
            // Do nothing if we add the same point.
            // Otherwise we get infinite recursion !
            if (point->equals(p))
                return;
                //*/

            // Save this data point that was here for a later re-insert
            SharedPoint oldPoint = point;
            point.reset();

            // Split the current node and create new empty trees for each
            // child octant.
            for (int i = 0 ; i < 8 ; ++i)
            {
                // Compute new bounding box for this child
                Vec3 newOrigin = origin;
                newOrigin.x += halfDimension.x * (i&4 ? 0.5 : -0.5);
                newOrigin.y += halfDimension.y * (i&2 ? 0.5 : -0.5);
                newOrigin.z += halfDimension.z * (i&1 ? 0.5 : -0.5);
                children[i] = std::make_shared<Octree>(newOrigin, halfDimension * .5);
            }

            // Re-insert the old point, and insert this new point
            // (We wouldn't need to insert from the root, because we already
            // know it's guaranteed to be in this section of the tree)

            //std::cout << "+>oldPoint : [" << oldPoint->x << ", " << oldPoint->y << ", " << oldPoint->z << "]" << std::endl;
            //std::cout << "+>p : [" << p->x << ", " << p->y << ", " << p->z << "]" << std::endl;

            //std::cout << "+>leaf findOctant(oldPoint) : " << findOctant(oldPoint) << std::endl;
            //std::cout << "+>leaf findOctant(p) : " << findOctant(p) << std::endl;

            children[findOctant(oldPoint)]->insert(oldPoint, depth + 1);
            children[findOctant(p)]->insert(p, depth + 1);
            ++count;
        }
    }
}


// This is a really simple routine for querying the tree for points
// within a bounding box defined by min/max points (bmin, bmax)
// All results are pushed into 'results'
void Octree::getPointsInsideBox(const Vec3& bmin, const Vec3& bmax, std::vector<SharedPoint>& results)
{
    // If we're at a leaf node, just see if the current data point is inside
    // the query bounding box
    if (isLeafNode())
    {
        if (point.get() != nullptr)
        {
            if (point->x > bmax.x || point->y > bmax.y || point->z > bmax.z)
                return;
            if (point->x < bmin.x || point->y < bmin.y || point->z < bmin.z)
                return;
            results.push_back(point);
        }
    }
    else
    {
        // We're at an interior node of the tree. We will check to see if
        // the query bounding box lies outside the octants of this node.
        for (int i = 0 ; i < 8 ; ++i)
        {
            // Compute the min/max corners of this child octant
            Vec3 cmax = children[i]->origin + children[i]->halfDimension;
            Vec3 cmin = children[i]->origin - children[i]->halfDimension;

            // If the query rectangle is outside the child's bounding box,
            // then continue
            if (cmax.x < bmin.x || cmax.y < bmin.y || cmax.z < bmin.z) continue;
            if (cmin.x > bmax.x || cmin.y > bmax.y || cmin.z > bmax.z) continue;

            // At this point, we've determined that this child is intersecting
            // the query bounding box
            children[i]->getPointsInsideBox(bmin, bmax, results);
        }
    }
}
