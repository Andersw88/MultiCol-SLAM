/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<ros/ros.h>
#include <cv_bridge/cv_bridge.h>

#include<opencv2/core/core.hpp>

#include"../include/cSystem.h"
#include <gperftools/profiler.h>

using namespace std;

class ImageGrabber
{
public:
    ImageGrabber(MultiColSLAM::cSystem* pSLAM):mpSLAM(pSLAM){}

    void GrabImage(const sensor_msgs::ImageConstPtr& msg);

    MultiColSLAM::cSystem* mpSLAM;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "Mono");
    ros::start();

    if(argc != 4)
    {
        cerr << endl << "Usage: rosrun MultiColSLAM Mono path_to_vocabulary path_to_settings" << endl;        
        ros::shutdown();
        return 1;
    }    

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    // MultiColSLAM::cSystem SLAM(argv[1],argv[2],MultiColSLAM::cSystem::MONOCULAR,true);

    MultiColSLAM::cSystem MultiSLAM(argv[1],argv[2], argv[3], true);

    ImageGrabber igb(&MultiSLAM);

    ros::NodeHandle nodeHandler;
    ros::Subscriber sub = nodeHandler.subscribe("/camera/image_raw", 1, &ImageGrabber::GrabImage,&igb);



    ProfilerStart("fisheye.prof");
    ros::spin();
    ProfilerStop();

    // Stop all threads
    MultiSLAM.Shutdown();

    // Save camera trajectory
    MultiSLAM.SaveMKFTrajectoryLAFIDA("KeyFrameTrajectory.txt");

    ros::shutdown();

    return 0;
}

void ImageGrabber::GrabImage(const sensor_msgs::ImageConstPtr& msg)
{
    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptr;
    // cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvShare(msg);
        // cv_ptr = cv_bridge::toCvCopy(msg,"CV_8UC1");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    } 
    
    // cv::Mat colorMat = cv_ptr->image;
    // cv::Mat greyMat;
    std::vector<cv::Mat> mt(1);

    cv::cvtColor(cv_ptr->image, mt[0], cv::COLOR_BGR2GRAY);

    // mt[0] = cv_ptr->image;

    mpSLAM->TrackMultiColSLAM(mt,cv_ptr->header.stamp.toSec());
    
}


