/*
 * Written by Andrea Pennisi
 */

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <opencv2/opencv.hpp>
#include "object_detector.h"
#include "camera.h"

using namespace socialdistancing::objectdetection;

namespace socialdistancing
{
    namespace utils
    {
        class Utility
        {
            public:
                /**
                 * @brief Constructor class Utility
                 */
                Utility() { ; }
                static float distance(const cv::Point2f& p1, const cv::Point2f& p2,
                    float dist);
                
                /**
                 * @brief make a mosaic between a set of images
                 * @param images a set of images
                 * @return an image containing a mosaic of all the input images
                 */
                static cv::Mat makeMosaic(const std::vector<cv::Mat>& images);
                
                /**
                 * @brief compute a hsv histogram
                 * @param img input image
                 * @param mask mask (if any) used for computing the histogram
                 * @param d bounding box representing the detection for which the histogram is computed
                 */
                static cv::Mat computeHist(const cv::Mat& img, const cv::Mat& mask, const bbox_t& d);
        };
    }
}

#endif
