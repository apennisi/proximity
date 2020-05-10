#ifndef _BG_SUBTRACTOR_
#define _BG_SUBTRACTOR_

#include <iostream>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudabgsegm.hpp>
#include <opencv2/video/background_segm.hpp>

namespace socialdistancing
{
  class BgSubtractor
  {
    public:
      BgSubtractor() { ; }
      BgSubtractor(int camNum);
      bool apply(const std::vector<cv::Mat>& frames, const float& learningRate = 0.01);
    public:
      inline std::vector<cv::Mat> getMasks() const
      {
        return cpuMasks;
      }
    private:
      std::vector<cv::Ptr<cv::BackgroundSubtractor>> bg;
      std::vector<cv::cuda::GpuMat> gpuFrames, gpuMasks;
      std::vector<cv::Mat> cpuMasks;
      int frameCounter;
  };
}

#endif
