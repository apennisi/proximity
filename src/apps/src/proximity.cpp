#include <cstdlib>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <stdexcept>
#include <system_error>

#include "object_detector.h"
#include <opencv2/cudabgsegm.hpp>
#include "camera.h"
#include "camerastack.h"
#include "dataType.h"
#include "object_detector.h"
#include "configmanager.h"
#include "homography.h"
#include "utility.h"
#include "tracker.h"
#include "FeatureTensor.h"
#include "utils.h"
#include "heatmap.h"
#include "bgsubtractor.h"



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

    cv::Ptr<cv::BackgroundSubtractor> bg = cv::cuda::createBackgroundSubtractorMOG();

    auto& params = config.deepSortParam();


    std::vector<std::shared_ptr<FeatureTensor>> featureTensor(config.getCameraNumber());
    
    auto i = 0;
    for(auto& feat : featureTensor)
    {
      feat = std::shared_ptr<FeatureTensor>(new FeatureTensor(params[i].metric_model(), params[i].feature_model()));
      ++i;
    }

    std::vector<std::shared_ptr<tracker>> deepTracker(config.getCameraNumber());
    i = 0;
    for(auto& t : deepTracker)
    {
      t = std::shared_ptr<tracker>( new tracker(params[i]) );
      ++i;
    }
       
    //initialize all the main objects
    CameraStack streams(config.getCameraParam());
    ObjectDetector detector(config.getDetectorParam());
    
    //storing variables
    std::vector<cv::Mat> frames;
    BgSubtractor bgSubtractors(config.getCameraNumber());
    std::vector<cv::Mat> detectionFrames(config.getCameraNumber());
    std::vector<cv::Mat> trackingFrames(config.getCameraNumber());
    std::vector< std::vector<bbox_t> > detections(config.getCameraNumber());
		std::vector< std::vector<bool> > safe_distance(config.getCameraNumber());
    std::vector< std::shared_ptr<HeatMap> > heatmaps(config.getCameraNumber());
    for(auto& h : heatmaps)
    {
      h = std::shared_ptr<HeatMap>(new HeatMap(cv::Size(1920, 1080)));
    }

    auto& cameras = streams.getCameraStack();
    bool vis = false;
    
    while(streams.getFrame(frames))
    {       
        std::stringstream ss;
        i = 0;
        
        for(const auto& frame : frames)
        {
            detectionFrames.at(i) = frame.clone();
            trackingFrames.at(i) = frame.clone();
            detector.classify(detectionFrames.at(i), false);
            detections.at(i) = detector.detections();
            i++;
        }

        if(bgSubtractors.apply(detectionFrames))
        {
          vis = true;
          const auto& bgMasks = bgSubtractors.getMasks();

          i = 0;
          for(const auto& detection : detections)
          { 
            DETECTIONS trackingDetections; 
            auto j = 0;
            for(const auto& d : detection)
            {
              DETECTION_ROW det;
              det.tlwh << d.x, d.y, d.w, d.h;
              det.confidence = d.prob;
              det.class_num = d.obj_id;
              cv::Point2f p2D(d.x + d.w*.5, d.y + d.h);
              trackingDetections.push_back(det);
              j++;
            }

            if(featureTensor[i]->getRectsFeature(detectionFrames.at(i), trackingDetections) == false) 
            {
              throw std::runtime_error("Tensorflow get feature failed");
            }

            deepTracker[i]->predict();
            deepTracker[i]->update(trackingDetections);
            std::vector<RESULT_DATA> result;
            for(auto& track : deepTracker[i]->tracks) 
            {
                if(!track.is_confirmed() || track.time_since_update > 1) continue;
                result.push_back(std::make_pair(std::make_pair(track.track_id, track.detection_class), std::make_pair(track.to_tlwh(), track.color)));
            }

            std::vector<cv::Point2f> feet3D(result.size());
            for(unsigned int k = 0; k < result.size(); k++) 
            {
              DETECTBOX tmp = result[k].second.first;
              cv::Point2f p2D(tmp(0) + tmp(2)*.5,tmp(1) + tmp(3));
              feet3D[k] = cameras[i].camera2world(p2D);
            }
            
            heatmaps[i]->apply(result, bgMasks[i], detectionFrames[i], config.minimumDistance()[i], feet3D);
          }
        }

        if(vis)
        {
          cv::imshow("DETECTIONS", Utility::makeMosaic(detectionFrames));
          cv::waitKey(1);
        }

    }
    return 0;
}
