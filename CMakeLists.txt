CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(OpenCV REQUIRED)

PROJECT(plane_detection)

ADD_EXECUTABLE(plane_detection src/PointCloud.cpp src/main.cpp)
TARGET_LINK_LIBRARIES(plane_detection ${OpenCV_LIBS})
