#include "Octree.h"

#include "Ransac.h"
#include <algorithm>

// Build a tree from a point cloud.
Octree::Octree(const PointCloud& cloud, unsigned int maxdepth) :
    mRoot(cloud.center(), cloud.halfDimension())
{
    for (auto&& p : cloud.points())
        mRoot.insert(p, maxdepth);
}


// Detect planes in the point cloud.
void Octree::detectPlanes(int depthThreshold, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generator, std::vector<SharedPlane>& planes, UnionFindPlanes& colors, double dCos) const
{
    std::vector<SharedPoint> pts;
    mRoot.detectPlanes(depthThreshold, epsilon, numStartPoints, numPoints, steps, generator, planes, colors, dCos, pts);
}


// Create a node.
Octree::Node::Node(const Vec3d& center, const Vec3d& halfSize) :
    center(center), halfSize(halfSize), count(0)
{
}

// Collect all the points in this subtree.
void Octree::Node::getPoints(std::vector<SharedPoint>& pts) const
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

void Octree::Node::detectPlanes(int depthThreshold, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generator, std::vector<SharedPlane>& planes, UnionFind<SharedPoint, std::pair<RGB, bool>>& colors, double dCos, std::vector<SharedPoint>& pts) const
{
    // Node => recursion.
    if (count > depthThreshold)
    {
        // Get planes from children.
        std::vector<SharedPlane> plns;
        for (auto&& child : children)
        {
            std::vector<SharedPoint> child_pts;
            if (child.get() != nullptr)
            {
                child->detectPlanes(depthThreshold, epsilon, numStartPoints, numPoints, steps, generator, plns, colors, dCos, child_pts);
                for (auto&& p : child_pts)
                    pts.push_back(p);
            }
        }

        /*
        // Remove planes that are too small.
        if (!plns.empty())
        {
            static auto comp = [](const SharedPlane& a, const SharedPlane& b){return a->getCount() > b->getCount();};
            std::sort(plns.begin(), plns.end(), comp);

            unsigned int minCount = plns[0]->getCount() / 200;
            //std::cout << "max count = " << plns[0]->getCount() << std::endl;
            for (SharedPlane& p : plns)
            {
                if (p->getCount() <= minCount)
                {
                    p->destroy(colors);
                    p.reset();
                }
            }
        }
        //*/

        // Try to merge planes.
        for (unsigned int i = 0 ; i < plns.size() ; ++i)
        {
            for (unsigned int j = 0 ; j < i ; ++j)
            {
                if (plns[i] && plns[j] && plns[i]->mergeableWith(*plns[j], dCos))
                {
                    plns[i]->merge(*plns[j], colors);
                    plns[j].reset();
                }
            }
        }

        // Try to match free points to planes.
        for (auto&& p : pts)
        {
            if (!colors.at(p).second)
            {
                // Get best plane for this point.
                std::vector<std::pair<SharedPlane, double> > dist;
                for (SharedPlane plane : plns)
                    if (plane && plane->accept(p))
                        dist.push_back(std::make_pair(plane, plane->squareDistance(p)));

                // We have a candidate.
                if (!dist.empty())
                {
                    std::sort(dist.begin(), dist.end(), [](const std::pair<SharedPlane, double>& a, const std::pair<SharedPlane, double>& b){ return a.second < b.second; });
                    dist[0].first->addPoint(p, colors);
                }
            }
        }

        // Recompute equations of planes (with new points).
        for (SharedPlane plane : plns)
        {
            if (plane)
            {
                plane->computeEquation();
                planes.push_back(plane);
            }
        }
    }
    // Small number of points => find planes.
    else
    {
        // Collect points.
        this->getPoints(pts);

        // Find a plane.
        for (int i = 0 ; i < 1 ; ++i)
        {
            SharedPlane plane = Ransac::ransac(pts, epsilon, numStartPoints, numPoints, steps, generator, colors);
            if (!plane)
                return;

            planes.push_back(plane);

            // Random color.
            std::uniform_int_distribution<int> distribution(0, 255);
            auto random = std::bind(distribution, generator);
            plane->setColor(RGB(random(), random(), random()), colors);
        }
    }
}


// Determine which octant of the tree contains p.
int Octree::Node::findOctant(SharedPoint p) const
{
    int oct = 0;
    if (p->x >= center.x) oct |= 4;
    if (p->y >= center.y) oct |= 2;
    if (p->z >= center.z) oct |= 1;
    return oct;
}

// Whether this is a leaf.
bool Octree::Node::isLeafNode() const
{
    return children[0].get() == nullptr;
}

// Insert a point with max recursion depth.
bool Octree::Node::insert(SharedPoint p, unsigned int depth)
{
    bool result = false;

    if (depth == 0)
    {
        std::cerr << "Max depth reached. Point not added : [" << p->x << ", " << p->y << ", " << p->z << "]" << std::endl;
        return result;
    }
    --depth;

    // Not a leaf => recursion.
    if (!isLeafNode())
        result = children[findOctant(p)]->insert(p, depth);
    // Leaf => insert here.
    else
    {
        // Empty leaf => set point.
        if (point.get() == nullptr)
        {
            point = p;
            result = true;
        }
        // Non-empty leaf => split it.
        else
        {
            SharedPoint oldPoint = point;
            point.reset();

            for (int i = 0 ; i < 8 ; ++i)
            {
                Vec3d newCenter = center;
                newCenter.x += halfSize.x * (i&4 ? 0.5 : -0.5);
                newCenter.y += halfSize.y * (i&2 ? 0.5 : -0.5);
                newCenter.z += halfSize.z * (i&1 ? 0.5 : -0.5);
                children[i] = std::make_shared<Node>(newCenter, halfSize / 2);
            }

            // Insert old point and new point.
            result = children[findOctant(oldPoint)]->insert(oldPoint, depth)
                    && children[findOctant(p)]->insert(p, depth);

            // If an error occured, reset to previous status.
            if (!result)
            {
                for (int i = 0 ; i < 8 ; ++i)
                    children[i].reset();
                point = oldPoint;
            }
        }
    }

    if (result)
        ++count;
    return result;
}
