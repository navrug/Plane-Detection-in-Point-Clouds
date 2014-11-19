#include "Ransac.h"

#include <set>
#include <functional>

#include <iostream>

Plane ransac(const std::vector<Point>& points, double epsilon, int numStartPoints, int numPoints, int steps, std::default_random_engine& generateur)
{
    Plane result(0,0,0,0);
    double score = -1;

    if (points.size() < numStartPoints) {
        std::cerr << "Pas assez de points pour le RANSAC" << std::endl;
        return result;
    }
    if (numStartPoints < 3) {
        std::cerr << "Choisissez au moins 3 points de depart pour le RANSAC" << std::endl;
        return result;
    }

    // Generateur aleatoire
    std::uniform_int_distribution<int> distribution(0, points.size() - 1);
    auto random = std::bind(distribution, generateur);

    for (int t = 0 ; t < steps ; ++t) {
        // Choisit les premiers points
        std::set<int> choisis;
        std::vector<Point> pts;
        for (int i = 0 ; i < numStartPoints ; ++i) {
            int k = random();
            while (choisis.find(k) != choisis.end())
                int k = random();

            choisis.insert(k);
            pts.push_back(points[k]);
        }

        Plane plan(pts[0], pts[1], pts[2]);

        for (int i = 0 ; i < points.size() ; ++i)
            if (choisis.find(i) == choisis.end() && plan.distance(points[i]) <= epsilon)
                pts.push_back(points[i]);

        if (pts.size() > numPoints) {
            double erreur = 0;
            for (int i = 0 ; i < pts.size() ; ++i)
                erreur += plan.distance(points[i]);

            plan.setPoints(pts);
            std::cout << plan << " : score = " << erreur << std::endl;

            if (score < 0 || erreur < score) {
                result = plan;
                result.setPoints(pts);
                score = erreur;
            }
        }
    }

    return result;
}
