cmake_minimum_required(VERSION 2.8.3)
project(multicol_slam)

# rosbuild_init()
find_package(catkin REQUIRED)

# IF(NOT ROS_BUILD_TYPE)
#   SET(ROS_BUILD_TYPE Release)
# ENDIF()

# MESSAGE("Build type: " ${ROS_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -march=native")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder -Wno-unused-variable -Wno-sequence-point") #TODO fix warnings

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)


# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/DBoW2 ${CMAKE_CURRENT_BINARY_DIR}/../ThirdParty/DBoW2/build )
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/../build )


find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

find_package(Eigen3 REQUIRED)
find_package(Pangolin REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  cmake_modules
  roscpp
  sensor_msgs
  image_transport
  cv_bridge
)

set(LIBS 
${OpenCV_LIBS} 
${EIGEN3_LIBS}
${Pangolin_LIBRARIES}
# ${PROJECT_SOURCE_DIR}/../ThirdParty/DBoW2/lib/libDBoW2.so
# ${PROJECT_SOURCE_DIR}/../ThirdParty/g2o/lib/libg2o.so
${PROJECT_SOURCE_DIR}/../lib/libMultiCol-SLAM.so
)

catkin_package(
  INCLUDE_DIRS ../include
  CATKIN_DEPENDS roscpp tf
  LIBRARIES multicol_slam ${LIBS}
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/../
  ${PROJECT_SOURCE_DIR}/../include
  ${PROJECT_SOURCE_DIR}/../ThirdParty
  ${PROJECT_SOURCE_DIR}/../ThirdParty/g2o
  ${PROJECT_SOURCE_DIR}/../ThirdParty/OpenGV/include
  ${Pangolin_INCLUDE_DIRS}
)

# Node for monocular camer
add_executable(fishEye
  src/ros_mono.cc 
)
# add_dependencies(fishEye multi_col_slam_lafida)



target_link_libraries(fishEye
  ${catkin_LIBRARIES}
  ${LIBS}
  profiler
)