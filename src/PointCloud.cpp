#include "PointCloud.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

void PointCloud::merge(const PointCloud& other)
{
    center *= points.size();
    for (SharedPoint p : other.points)
        addPoint(p, other.colors.at(p));
    finishLoad();
}

void PointCloud::addPoint(SharedPoint p, RGB color)
{
    center += *p;
    max.max(*p);
    min.min(*p);

    points.push_back(p);
    colors.append(p, color);
}

void PointCloud::finishLoad()
{
    center /= points.size();
    halfDimension = (max - min) / 2;
    std::cout << "Created a PointCloud of " << points.size() << " points." << std::endl;
}

PointCloud::PointCloud()
{
    min = Vec3(
                std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity());
    max = Vec3(
                -std::numeric_limits<double>::infinity(),
                -std::numeric_limits<double>::infinity(),
                -std::numeric_limits<double>::infinity());
}

void PointCloud::loadPly(const std::string& filename)
{
    std::ifstream infile(filename.c_str());

    std::string line;
    std::getline(infile, line);
    std::getline(infile, line);

    int size;
    std::istringstream iss(line);
    if (!(iss >> size)) {
        std::cerr << "Error : size missing.";
        return;
    }

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        double x, y, z;
        unsigned int r, g, b;
        if (!(iss >> x >> y >> z >> r >> g >> b))
            break; // error

        this->addPoint(std::make_shared<Point>(x, y, z), RGB(r, g, b));
    }

    this->finishLoad();
}

void PointCloud::load3D(const std::string& filename)
{
    std::ifstream infile(filename.c_str());
    std::string line;

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        double x, y, z;
        if (!(iss >> x >> y >> z))
            break; // error

        this->addPoint(std::make_shared<Point>(x, y, z), RGB(128, 128, 128));
    }

    this->finishLoad();
}

bool PointCloud::toPly(const std::string& filename, bool showPlanes)
{
    std::ofstream out(filename.c_str());
    if (!out.is_open()) {
        std::cerr << "Cannot save " << filename << std::endl;
        return false;
    }

    out << "ply" << std::endl
        << "format ascii 1.0" << std::endl
        << "element vertex " << points.size() << std::endl
        << "property float x" << std::endl
        << "property float y" << std::endl
        << "property float z" << std::endl
        << "property uchar red" << std::endl
        << "property uchar green" << std::endl
        << "property uchar blue" << std::endl
        << "end_header" << std::endl;

    for (SharedPoint p : points) {
        RGB rgb;
        if (showPlanes)
            rgb = colors.at(p);
        else
            rgb = p->inPlane ? RGB(255, 255, 255) : RGB(0, 0, 0);
        out << p->x << " " << p->y << " " << p->z << " " << int(rgb.r) << " " << int(rgb.g) << " " << int(rgb.b) << " " << std::endl;
    }

    out.close();
    std::cout << "Ply " << filename << " exported." << std::endl;
    return true;
}
