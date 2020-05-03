#include "utility.h"
#include "camera.h"

using namespace socialdistancing::utils;

float
Utility::distance(const cv::Point2f& p1, const cv::Point2f& p2, float dist)
{
  cv::Point2f p = p1 - p2;
	auto edist = std::sqrt(p.x*p.x + p.y*p.y);
	return edist < dist + 0.50; //1.5meters
}

cv::Mat
Utility::makeMosaic(const std::vector<cv::Mat> &images)
{
    auto numImgs = images.size();
    if(numImgs > 0)
    {
        const auto& sz = images.at(0).size();
        cv::Size newSize(sz.width*numImgs, sz.height);
        cv::Mat newImg(newSize, CV_8UC3);
        auto wIdx = 0;
        for(const auto& im : images)
        {
            im.copyTo(newImg(cv::Rect(wIdx, 0, im.cols, im.rows)));
            wIdx += im.cols;
        }
        return newImg;
    }
    return cv::Mat();
}


cv::Mat 
Utility::computeHist(const cv::Mat& img, const cv::Mat& mask, const bbox_t& d)
{
    if(img.empty())
    {
        throw std::invalid_argument("Error: the image is empty");
    }
    
    auto rect = [](const bbox_t& r, const cv::Size& sz)
    {
        cv::Rect rectangle(r.x, r.y, r.w, r.h);
        if(rectangle.x < 0) rectangle.x = 0;
        if(rectangle.y < 0) rectangle.y = 0;
        if(rectangle.width + rectangle.x  > sz.width ) rectangle.width -= (((rectangle.width + rectangle.x) - sz.width ) + 1);
        if(rectangle.height + rectangle.y > sz.height ) rectangle.height -= (((rectangle.height + rectangle.y) - sz.height )  + 1);
        return rectangle;
    };
    
    const auto& r = rect(d, img.size());
    const auto& patch = img(r);
    cv::Mat maskPatch;
    if(!mask.empty())
    {
        maskPatch = mask(r);
    }
    
    cv::Mat hsvPatch;
    cv::cvtColor(patch, hsvPatch, CV_BGR2HSV);
    static const cv::Mat noMask;
    static const int     hueBins        = 50;
    static const int     satBins        = 60;
    static const int     bins[]         = {hueBins, satBins};
    static const float   hueRanges[]    = {0, 256};
    static const float   satRanges[]    = {0, 180};
    static const float  *ranges[]       = {hueRanges, satRanges};
    static const int     imageCount     = 1;
    static const int     channels[]     = {0, 1};
    static const int     dimensionCount = 2;
    static const bool    uniform        = true;
    static const bool    accumulate     = false;
    cv::Mat hist;
    cv::calcHist(&hsvPatch, imageCount, channels, maskPatch, hist,
                 dimensionCount, bins, ranges, uniform, accumulate);
    static const double  alpha          = 0.0;
    static const double  beta           = 1.0;
    static const int     normKind       = cv::NORM_MINMAX;
    static const int     dtype          = -1;
    cv::normalize(hist, hist, alpha, beta, normKind, dtype, noMask);
    return hist;
}
