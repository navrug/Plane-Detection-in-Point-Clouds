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
        unsigned char r, g, b;
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
        addPoint(Point(x, y, z, r, g, b));
        count++;
//        cout << x << " " << y << " " << z << " added." << endl;
    }
    middle = Vec3(xAvg/count, yAvg/count, zAvg/count);
    halfDimension = Vec3((xMax - xMin)/2, (yMax - yMin)/2, (zMax - zMin)/2);
    cout << "Created a PointCloud of " << count << " points." << endl;
}

/*void toPly(const Image<uchar>& I1, const Mat& D, int n, String filename)
{
    int h(I1.height()), w(I1.width());
    vector<Point3f> vertices = vector<Point3f>();
    vector<Vec3i> faces = vector<Vec3i>();
    vector<Vec3b> color = vector<Vec3b>();
    int bound = 80;
    for(int i(n); i<h-n; ++i)
        for (int j(n); j<w-n; ++j) {
            if(D.at<char>(i, j)<-bound || D.at<char>(i, j)>bound) {
                vertices.push_back(Point3f(j, i, 100000.0/(100.0-bound)));
            } else {
                vertices.push_back(Point3f(j, i, 100000.0/(100.0+D.at<char>(i, j))));
            }
            color.push_back(Vec3b(I1.at<uchar>(i, j),I1.at<uchar>(i, j),I1.at<uchar>(i, j)));
            if(i<h-n-1 && j <w-n-1) {
                faces.push_back(Vec3i((int)((i-n)*(w-2*n) + (j-n)), (int)((i-n+1)*(w-2*n) + (j-n)), (int)((i-n)*(w-2*n) + (j-n+1))));
                faces.push_back(Vec3i((int)((i-n+1)*(w-2*n) + (j-n)), (int)((i-n)*(w-2*n) + (j-n+1)), (int)((i-n+1)*(w-2*n) + (j-n+1))));
            }
        }
    savePly(filename+".ply", vertices, faces, color);
}*/
