#include "Ransac.h"
#include "PlaneSet.h"

#include <set>
#include <functional>

#include <iostream>

SharedPlane Ransac::ransac(std::vector<SharedPoint>& points, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generateur, UnionFind<SharedPoint, RGB>& colors)
{
    SharedPlane result;
    double score = -1;

    if (points.size() < numStartPoints) {
        //std::cerr << "Pas assez de points pour le RANSAC" << std::endl;
        return result;
    }
    if (numStartPoints < 3) {
        //std::cerr << "Choisissez au moins 3 points de depart pour le RANSAC" << std::endl;
        return result;
    }

    // On calcule la l'écart-type de la distance entre les points
    Vec3 center;
    for (auto&& point : points)
        center += *point;
    center /= points.size();

    double stddev = 0;
    for (auto&& point : points)
        stddev += center.squareDistance(*point);
    stddev /= points.size();
    stddev = std::sqrt(stddev);

    // Le seuil de distance est proportionnel à cet écart-type.
    epsilon *= stddev;


    std::vector<SharedPoint> result_pts;

    for (int t = 0 ; t < steps ; ++t) {
        // Choisit les premiers points
        std::vector<SharedPoint> pts;
        for (int i = 0 ; i < numStartPoints ; ++i) {
            // Generateur aleatoire
            std::uniform_int_distribution<int> distribution(i, points.size() - 1);
            int k = distribution(generateur);

            std::swap(points[i], points[k]);
            pts.push_back(points[i]);
        }

        SharedPlane shared_plan = std::make_shared<Plane>(pts);
        Plane& plan = *shared_plan;

        pts.clear();
        for (auto&& p : points)
            if (plan.distance(p) <= epsilon)
                pts.push_back(p);

        if (pts.size() > numPoints) {
            plan.setPoints(pts);

            double erreur = 0;
            for (auto&& p : pts)
                erreur += plan.distance(p);

            //std::cout << plan << " : score = " << erreur << std::endl;

            if (score < 0 || erreur < score) {
                result = shared_plan;
                result_pts = pts;
                score = erreur;
            }
        }
    }

    for (auto&& p : result_pts)
    {
        colors.merge(p, result_pts[0]);
        p->inPlane = true;
    }

    return result;
}
