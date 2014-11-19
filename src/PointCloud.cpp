#include "PointCloud.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

PointCloud::PointCloud()
{
    //ctor
}


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
        if (!(iss >> x >> y >> z)) { break; } // error
        xAvg += x;
        yAvg += y;
        zAvg += z;
        xMax = max(xMax, x);
        yMax = max(yMax, y);
        zMax = max(zMax, z);
        xMin = min(xMin, x);
        yMin = min(yMin, y);
        zMin = min(zMin, z);
        addPoint(Point(x, y, z));
        count++;
//        cout << x << " " << y << " " << z << " added." << endl;
    }
    middle = Vec3(xAvg/count, yAvg/count, zAvg/count);
    halfDimension = Vec3((xMax - xMin)/2, (yMax - yMin)/2, (zMax - zMin)/2);
    cout << "Created a PointCloud of " << count << " points." << endl;
}

PointCloud::~PointCloud()
{
    //dtor
}
