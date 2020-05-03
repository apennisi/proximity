#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "object_detector.h"
#include "camera.h"
#include "camerastack.h"
#include "object_detector.h"
#include "configmanager.h"
#include "homography.h"
#include "utility.h"


using namespace socialdistancing;
using namespace socialdistancing::config;
using namespace socialdistancing::utils;
using namespace socialdistancing::objectdetection;


auto main(int argc, char **argv) -> int
{
    if(argc != 2)
    {
        std::cout << "Error: too few/much arguments!" << std::endl;
        std::cout << "Usage: " << argv[0] << " /path/to/the/config/file" << std::endl;
        exit(-1);
    }
    
    ConfigManager config;
    config.read(std::string(argv[1]));
    config.print();
       
    //initialize all the main objects
    CameraStack streams(config.getCameraParam());
    ObjectDetector detector(config.getDetectorParam());
    
    //storing variables
    std::vector<cv::Mat> frames;
    std::vector<cv::Mat> detectionFrames(config.getCameraNumber());
    std::vector< std::vector<bbox_t> > detections(config.getCameraNumber());
		std::vector< std::vector<bool> > safe_distance(config.getCameraNumber());
    
    auto& cameras = streams.getCameraStack();
    
    while(streams.getFrame(frames))
    {       
        std::stringstream ss;
        auto i = 0;
        
        for(const auto& frame : frames)
        {
            detectionFrames.at(i) = frame.clone();
            detector.classify(detectionFrames.at(i), false);
            detections.at(i) = detector.detections();
            i++;
        }
 

        i = 0;
				for(const auto& detection : detections)
        { 
					std::vector<cv::Point2f> feet3D(detection.size());
					auto j = 0;
          for(const auto& d : detection)
          {
						cv::Point2f p2D(d.x + d.w*.5, d.y + d.h);
						feet3D[j] = cameras[i].camera2world(p2D);
						j++;
					}
					
					safe_distance[i].clear();
					safe_distance[i].resize(detection.size(), false);
					float minDist = config.minimumDistance()[i];
					for(auto l = 0; l < int(feet3D.size() - 1); ++l)
					{
						for(auto m = l+1; m < int(feet3D.size()); ++m)
						{
							if(Utility::distance(feet3D[l], feet3D[m], minDist))
							{
								safe_distance[i][m] = safe_distance[i][l] = true;
							}
						}
					}

          j = 0;
					for(const auto& d : detection)
					{
						cv::Scalar color;
						std::stringstream ss;
						if(safe_distance[i][j])
						{
							color = cv::Scalar(0, 0, 255);
							ss << "Abnormal";
						}
						else
						{
							color = cv::Scalar(0, 255, 0);
							ss << "Normal";
						}
						
						std::string obj_name = ss.str();
						cv::Size const text_size = cv::getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
						int const max_width = (text_size.width > (int)d.w + 2) ? text_size.width : (d.w + 2);
						cv::rectangle(detectionFrames[i], cv::Point2f(std::max((int)d.x - 1, 0), std::max((int)d.y - 30, 0)),
									cv::Point2f(std::min((int)d.x + max_width, detectionFrames[i].cols-1), std::min((int)d.y, detectionFrames[i].rows-1)),
									color, cv::FILLED, 8, 0);
						cv::putText(detectionFrames[i], obj_name, cv::Point2f(d.x, d.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
            cv::Rect r(d.x, d.y, d.w, d.h);
            cv::rectangle(detectionFrames[i], r, color, 2);
						j++;
					}
          i++;
        }

        cv::imshow("DETECTIONS", Utility::makeMosaic(detectionFrames));
            
        cv::waitKey(1);
    }
        
        return 0;
}
