CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(OpenCV REQUIRED)

PROJECT(plane_detection)

INCLUDE_DIRECTORIES(include)
ADD_EXECUTABLE(
    plane_detection
    include/Octree.h
    include/Plane.h
    include/Point.h
    include/PointCloud.h
    include/Polygon.h
    include/Vec3.h
    src/PointCloud.cpp
    src/main.cpp
)

TARGET_LINK_LIBRARIES(plane_detection ${OpenCV_LIBS})
