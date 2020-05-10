#include "bgsubtractor.h"

using namespace socialdistancing;

BgSubtractor::BgSubtractor(int camNum)
{
  gpuFrames.resize(camNum);
  gpuMasks.resize(camNum);
  cpuMasks.resize(camNum);
  frameCounter = 0;
  bg.resize(camNum);
  for(auto& b : bg)
  {
    b = cv::cuda::createBackgroundSubtractorMOG();
  }

}

bool BgSubtractor::apply(const std::vector<cv::Mat>& frames, const float& learningRate)
{
  auto i = 0;
  for(const auto& frame : frames)
  {
    gpuFrames[i].upload(frame);
    bg[i]->apply(gpuFrames[i], gpuMasks[i], learningRate); 
    ++i;
  } 

  i = 0;
  for(auto& gpuFrame : gpuFrames)
  {
    gpuMasks[i].download(cpuMasks[i]);
    gpuFrame.release();
    gpuMasks[i].release();
    ++i;
  }

  return frameCounter++ > 10;
}
