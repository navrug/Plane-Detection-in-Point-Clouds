CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(OpenCV REQUIRED)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

PROJECT(plane_detection)

INCLUDE_DIRECTORIES(include)
ADD_EXECUTABLE(
    plane_detection
    include/Octree.h
    include/Plane.h
    include/Point.h
    include/PointCloud.h
    include/Polygon.h
    include/Ransac.h
    include/RGB.h
    include/Vec3.h
    src/Octree.cpp
    src/Plane.cpp
    src/PointCloud.cpp
    src/Ransac.cpp
    src/RGB.cpp
    src/main.cpp
)

TARGET_LINK_LIBRARIES(plane_detection ${OpenCV_LIBS})
