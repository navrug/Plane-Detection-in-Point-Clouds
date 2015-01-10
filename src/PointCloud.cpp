#include "PointCloud.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

void PointCloud::addPoint(SharedPoint p, RGB color)
{
    points.push_back(p);
    colors.append(p, color);
}

bool hasEnding (string const &fullString, string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

enum Extension { PLY, XYZ, TROISD };


PointCloud::PointCloud(const std::string& filename)
{
    Extension ext;
    if (hasEnding(filename, ".xyz"))
        ext = XYZ;
    else if (hasEnding(filename, ".3d"))
        ext = TROISD;
    else
        throw 0;

    ifstream infile(filename.c_str());
    string line;
    getline(infile, line);
    getline(infile, line);
    int size;
    istringstream iss(line);
    // Handling preamble
    switch (ext) {
        case TROISD:
            if (!(iss >> size)) {
                cout << "Error : size missing.";
                return;
            }; break;
    }

    int count(0);
    double xAvg(0);
    double yAvg(0);
    double zAvg(0);
    double xMax = -numeric_limits<double>::infinity();
    double yMax = -numeric_limits<double>::infinity();
    double zMax = -numeric_limits<double>::infinity();
    double xMin = numeric_limits<double>::infinity();
    double yMin = numeric_limits<double>::infinity();
    double zMin = numeric_limits<double>::infinity();
    while (getline(infile, line))
    {
        istringstream iss(line);
        double x, y, z, a;
        unsigned int r(0), g(0), b(0);
        switch (ext) {
            case XYZ: if (!(iss >> x >> y >> z >> r >> g >> b)) { return; }; break;
            case TROISD: if (!(iss >> x >> y >> z >> a)) { return; }; break;
        }
        xAvg += x;
        yAvg += y;
        zAvg += z;
        xMax = max(xMax, x);
        yMax = max(yMax, y);
        zMax = max(zMax, z);
        xMin = min(xMin, x);
        yMin = min(yMin, y);
        zMin = min(zMin, z);
        addPoint(std::make_shared<Point>(x, y, z), RGB(r, g, b));
        count++;
    }
    middle = Vec3(xAvg/count, yAvg/count, zAvg/count);
    halfDimension = Vec3((xMax - xMin)/2, (yMax - yMin)/2, (zMax - zMin)/2);
    cout << "Created a PointCloud of " << count << " points." << endl;
}

bool PointCloud::toPly(const std::string& filename, bool showPlanes)
{
    ofstream out(filename.c_str());
    if (!out.is_open()) {
        cout << "Cannot save " << filename << endl;
        return false;
    }

    out << "ply" << endl
        << "format ascii 1.0" << endl
        << "element vertex " << points.size() << endl
        << "property float x" << endl
        << "property float y" << endl
        << "property float z" << endl
        << "property uchar red" << endl
        << "property uchar green" << endl
        << "property uchar blue" << endl
        << "end_header" << endl;

    for (SharedPoint p : points) {
        RGB rgb;
        if (showPlanes)
            rgb = colors.at(p);
        else
            rgb = p->inPlane ? RGB(255, 255, 255) : RGB(0, 0, 0);
        out << p->x << " " << p->y << " " << p->z << " " << int(rgb.r) << " " << int(rgb.g) << " " << int(rgb.b) << " " << endl;
    }

    out.close();
    cout << "Ply " << filename << " exported." << endl;
    return true;
}
