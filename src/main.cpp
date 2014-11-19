#include <iostream>
#include "PointCloud.h"
#include "Octree.h"

using namespace std;

int main()
{
    PointCloud cloud("extract.xyz");
    Octree octree(cloud);
    cout << "Hello world!" << endl;
    return 0;
}
