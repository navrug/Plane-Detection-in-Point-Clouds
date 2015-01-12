#include "Ransac.h"

// Find a plane with RANSAC algorithm.
SharedPlane Ransac::ransac(std::vector<SharedPoint>& points, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generator, UnionFindPlanes& colors)
{
    SharedPlane result;
    if (points.size() < numStartPoints || numStartPoints < 3)
        return result;

    // Compute caracteristics of the point set in order to fit the parameters.
    // Center of the point set.
    Vec3d center;
    // Variance in each direction.
    Vec3d meansq;

    for (auto&& point : points)
    {
        center += *point;
        meansq += point->cmul(*point);
    }

    center /= points.size();
    meansq /= points.size();

    Vec3d stddev = meansq - center.cmul(center);

    // Approximate radius of the set.
    double radius = std::sqrt(stddev.x + stddev.y + stddev.z);

    // Thickness threshold is proportional to this radius.
    epsilon *= radius;


    // Points in the resulting plane.
    std::vector<SharedPoint> result_pts;
    std::vector<SharedPoint> remaining_pts;
    double score = -1;

    // Try steps times.
    for (int t = 0 ; t < steps ; ++t) {
        // Select the first points.
        std::vector<SharedPoint> pts;
        for (int i = 0 ; i < numStartPoints ; ++i) {
            // Random distribution
            std::uniform_int_distribution<int> distribution(i, points.size() - 1);
            int k = distribution(generator);

            std::swap(points[i], points[k]);
            pts.push_back(points[i]);
        }

        // Plane that fit to these points.
        SharedPlane shared_plane = std::make_shared<Plane>(pts);
        Plane& plane = *shared_plane;

        // Reset the point set and match all the points.
        std::vector<SharedPoint> pts_out;
        matchPoints(points, plane, epsilon, pts, pts_out);

        // If enough points.
        if (pts.size() > numPoints)
        {
            // Recompute the plane with these points.
            plane.setPoints(pts);

            // Compute the error for these points.
            double error = 0;
            for (auto&& p : pts)
                error += plane.squareDistance(p);

            // We improved the error.
            if (score < 0 || error < score)
            {
                result = shared_plane;
                result_pts = pts;
                remaining_pts = pts_out;
                score = error;
            }
        }
    }

    // Merge all the points together.
    for (auto&& p : result_pts)
    {
        colors.merge(p, result_pts[0]);
        //p->inPlane = true;
    }

    points = remaining_pts;
    return result;
}

// Match points close to the plane.
void Ransac::matchPoints(const std::vector<SharedPoint>& points, Plane& plane, double epsilon, std::vector<SharedPoint>& pts_in, std::vector<SharedPoint>& pts_out)
{
    pts_in.clear();
    pts_out.clear();
    for (auto&& p : points)
    {
        if (plane.squareDistance(p) <= epsilon)
            pts_in.push_back(p);
        else
            pts_out.push_back(p);
    }
}
