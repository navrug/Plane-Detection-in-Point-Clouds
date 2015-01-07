#include "PointCloud.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

PointCloud::PointCloud(const std::string& filename)
{
    ifstream infile(filename.c_str());
    string line;
    getline(infile, line);
    getline(infile, line);
    int size;
    istringstream iss(line);
    if (!(iss >> size)) {
        cout << "Error : size missing.";
        return;
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
        double x, y, z;
        unsigned int r, g, b;
        if (!(iss >> x >> y >> z >> r >> g >> b)) { break; } // error
        xAvg += x;
        yAvg += y;
        zAvg += z;
        xMax = max(xMax, x);
        yMax = max(yMax, y);
        zMax = max(zMax, z);
        xMin = min(xMin, x);
        yMin = min(yMin, y);
        zMin = min(zMin, z);
        addPoint(Point(x, y, z, new RGB(r, g, b)));
        count++;
    }
    middle = Vec3(xAvg/count, yAvg/count, zAvg/count);
    halfDimension = Vec3((xMax - xMin)/2, (yMax - yMin)/2, (zMax - zMin)/2);
    cout << "Created a PointCloud of " << count << " points." << endl;
}

bool PointCloud::toPly(const std::string& filename)
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

    for (int i = 0 ; i < points.size() ; ++i) {
        const Point& p = *points[i];
        out << p.x << " " << p.y << " " << p.z << " " << int(p.rgb->r) << " " << int(p.rgb->g) << " " << int(p.rgb->b) << " " << endl;
    }

    out.close();
    cout << "Ply " << filename << " exported." << endl;
    return true;
}
