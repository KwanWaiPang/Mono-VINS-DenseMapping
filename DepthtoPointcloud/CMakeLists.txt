cmake_minimum_required(VERSION 2.8.3)
project(DepthtoPointcloud_node)

find_package(catkin_simple REQUIRED)

# catkin_simple()
catkin_simple(ALL_DEPS_REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")

find_package(OpenCV REQUIRED)


# make libraries
cs_add_executable(${PROJECT_NAME} src/DepthtoPointcloud_node.cpp)

# link required libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})

cs_install()
cs_export() #要添加这个才可以作为别的包构建时的依赖？
