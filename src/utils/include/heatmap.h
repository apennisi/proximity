#ifndef _HEATMAP_H_
#define _HEATMAP_H_

#include "dataType.h"
#include <opencv2/core/matx.hpp>
#include <opencv2/opencv.hpp>
#include "utility.h"

namespace socialdistancing
{
  class HeatMap
  {
    public:
      HeatMap() { ; }
      HeatMap(const cv::Size& size);
      void apply(const std::vector<RESULT_DATA>& tracks, const cv::Mat& bgMasks, cv::Mat& in_out, float minDist, std::vector<cv::Point2f>& feet3D);
    private:
      cv::Mat colorMap(const cv::Mat& bgMask, cv::Mat& accuMat, const cv::Scalar& color);
    private:
      cv::Mat greenAcc, redAcc;
      cv::Size s;
  };
}

#endif
