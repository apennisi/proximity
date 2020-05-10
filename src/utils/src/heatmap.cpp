#include "heatmap.h"
#include "dataType.h"
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace socialdistancing;
using namespace socialdistancing::utils;

HeatMap::HeatMap(const cv::Size& size)
  : s(size)
{
  greenAcc = cv::Mat(size, CV_8UC1, cv::Scalar(0));
  redAcc = cv::Mat(size, CV_8UC1, cv::Scalar(0));
}


void HeatMap::apply(const std::vector<RESULT_DATA>& tracks, const cv::Mat& bgMasks, cv::Mat& in_out, float minDist, std::vector<cv::Point2f>& feet3D)
{
  std::vector<bool> safe_distance(tracks.size(), true);

  for(auto l = 0; l < int(tracks.size() - 1); ++l)
  {
    for(auto m = l+1; m < int(tracks.size()); ++m)
    {
      if(Utility::distance(feet3D[l], feet3D[m], minDist))
      {
        safe_distance[m] = safe_distance[l] = false;
      }
    }
  }

  auto i = 0;
  cv::Mat greenMask, redMask;
  greenMask = cv::Mat::zeros(bgMasks.size(), bgMasks.type());
  redMask = greenMask.clone();
  
  for(const auto& dist : safe_distance)
  {
    const auto& tmp = tracks[i].second.first;
    cv::Rect r(tmp(0), tmp(1), tmp(2), tmp(3));
    bool is_inside = (r & cv::Rect(0, 0, bgMasks.cols, bgMasks.rows)) == r;
    if(!is_inside)
    {
      if(r.x < 0) r.x = 0;
      if(r.y < 0) r.y = 0;
      if(r.x + r.width > bgMasks.cols) r.width -= (r.x + r.width - bgMasks.cols);
      if(r.y + r.height > bgMasks.rows) r.height -= (r.y + r.height - bgMasks.rows);
    }
    if(dist)
    {
      bgMasks(r).copyTo(greenMask(r));
    }
    else
    {
      bgMasks(r).copyTo(redMask(r));
    }

    ++i;  
  }

  auto greenColorMap = colorMap(greenMask, greenAcc, cv::Scalar(0., 255., 0.));
  auto redColorMap = colorMap(redMask, redAcc, cv::Scalar(0., 0., 255.));
	auto totalColorMap = greenColorMap + redColorMap;

  cv::addWeighted(totalColorMap, 0.4, in_out, 0.6, 10, in_out);

  i = 0;
  for(const auto& dist : safe_distance)
  {
    const auto& tmp = tracks[i].second.first;
    cv::Rect r(tmp(0), tmp(1), tmp(2), tmp(3));
    bool is_inside = (r & cv::Rect(0, 0, bgMasks.cols, bgMasks.rows)) == r;
    if(!is_inside)
    {
      if(r.x < 0) r.x = 0;
      if(r.y < 0) r.y = 0;
      if(r.x + r.width > bgMasks.cols) r.width -= (r.x + r.width - bgMasks.cols);
      if(r.y + r.height > bgMasks.rows) r.height -= (r.y + r.height - bgMasks.rows);
    }
    cv::Scalar color;
    std::string obj_name;
    if(dist)
    {
      obj_name = "Normal";
      color = cv::Scalar(0, 255, 0);
    }
    else
    {
      obj_name = "Abnormal";
      color = cv::Scalar(0, 0, 255);
    }
    
    cv::Size const text_size = cv::getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
		int const max_width = (text_size.width > (int)r.width + 2) ? text_size.width : (r.width + 2);
		cv::rectangle(in_out, cv::Point2f(std::max((int)r.x - 1, 0), std::max((int)r.y - 30, 0)),
		cv::Point2f(std::min((int)r.x + max_width, in_out.cols-1), std::min((int)r.y, in_out.rows-1)), color, cv::FILLED, 8, 0);
		cv::putText(in_out, obj_name, cv::Point2f(r.x, r.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);

    cv::rectangle(in_out, r, color, 2);
    ++i;  

  }
}

cv::Mat HeatMap::colorMap(const cv::Mat& bgMask, cv::Mat& accuMat, const cv::Scalar& color)
{
  cv::Mat tmp;
  cv::threshold(bgMask, tmp, 2, 2, cv::THRESH_BINARY);

  cv::add(accuMat, tmp, accuMat);
  cv::Mat normMat, normMat3;

  accuMat.convertTo(tmp, CV_32F);
  
  cv::normalize(tmp, normMat, 0, 1, cv::NORM_MINMAX);
  cv::Mat in[] = {normMat, normMat, normMat};
  cv::merge(in, 3, normMat3);

  cv::Mat colorMap;
  colorMap = cv::Mat(accuMat.size(), CV_32FC3, color);

  cv::multiply(colorMap, normMat3, colorMap);
  colorMap.convertTo(colorMap, CV_8UC3);

  return colorMap;
}
