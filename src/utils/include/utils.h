/*
 * Written by Andrea Pennisi
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include "dataType.h"

typedef std::vector<uint> UIntVec;
typedef std::vector<UIntVec> UIntMat;
typedef std::vector<bool> BoolVec;
typedef std::vector<cv::Point> Points;
typedef std::vector<cv::Mat> MatVec;

class DETECTION_ROW 
{
    public:
        DETECTBOX tlwh; //np.float
        float confidence; //float
        int class_num;
        cv::Scalar color;
        FEATURE feature; //np.float32
        DETECTBOX to_xyah() const;
        DETECTBOX to_tlbr() const;
};

typedef std::vector<DETECTION_ROW> DETECTIONS;


#endif
