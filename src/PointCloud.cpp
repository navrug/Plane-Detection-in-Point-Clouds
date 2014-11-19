#include "PointCloud.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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
    while (getline(infile, line))
    {
        istringstream iss(line);
        double x, y, z;
        if (!(iss >> x >> y >> z)) { break; } // error
        addPoint(Point(x, y, z));
        //cout << x << " " << y << " " << z << " added." << endl;
    }
}

PointCloud::~PointCloud()
{
    //dtor
}
