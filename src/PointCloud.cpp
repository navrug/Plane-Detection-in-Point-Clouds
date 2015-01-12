#include "PointCloud.h"

#include <fstream>

// Empty cloud.
PointCloud::PointCloud()
{
    min = Vec3d(
                std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity());
    max = Vec3d(
                -std::numeric_limits<double>::infinity(),
                -std::numeric_limits<double>::infinity(),
                -std::numeric_limits<double>::infinity());
}


// Load from file.
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

    this->boundingBox();
}

// Load from file.
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

    this->boundingBox();
}

// Save to file.
bool PointCloud::toPly(const std::string& filename, bool showPlanes) const
{
    std::ofstream out(filename.c_str());
    if (!out.is_open()) {
        std::cerr << "Cannot save " << filename << std::endl;
        return false;
    }

    out << "ply" << std::endl
        << "format ascii 1.0" << std::endl
        << "element vertex " << mPoints.size() << std::endl
        << "property float x" << std::endl
        << "property float y" << std::endl
        << "property float z" << std::endl
        << "property uchar red" << std::endl
        << "property uchar green" << std::endl
        << "property uchar blue" << std::endl
        << "end_header" << std::endl;

    for (SharedPoint p : mPoints) {
        std::pair<RGB, bool> state = mColors.at(p);
        RGB rgb;
        if (showPlanes)
            rgb = state.first;
        else
            rgb = state.second ? RGB(255, 255, 255) : RGB(0, 0, 0);
        out << p->x << " " << p->y << " " << p->z << " " << int(rgb.r) << " " << int(rgb.g) << " " << int(rgb.b) << " " << std::endl;
    }

    out.close();
    std::cout << "Ply " << filename << " exported." << std::endl;
    return true;
}


// Merge two point clouds.
void PointCloud::merge(const PointCloud& other)
{
    mCenter *= mPoints.size();
    for (SharedPoint p : other.mPoints)
        addPoint(p, other.mColors.at(p).first);
    this->boundingBox();
}


// Add point.
void PointCloud::addPoint(SharedPoint p, RGB color)
{
    mCenter += *p;
    max.max(*p);
    min.min(*p);

    mPoints.push_back(p);
    mColors.append(p, std::make_pair(color, false));
}

// Compute bounding box.
void PointCloud::boundingBox()
{
    mCenter /= mPoints.size();
    mHalfDimension = (max - min) / 2;
    std::cout << "Created a PointCloud of " << mPoints.size() << " points." << std::endl;
}
