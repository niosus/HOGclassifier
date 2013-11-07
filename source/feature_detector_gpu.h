#ifndef FEATURE_DETECTOR_GPU_H
#define FEATURE_DETECTOR_GPU_H

#include "feature_detector.h"
#include <vector>
#include <string>
#include <map>
#include "cv.h"
#include "opencv2/gpu/gpu.hpp"

class FeatureDetectorGpu: public FeatureDetector
{
public:
  FeatureDetectorGpu(SampleShape shape): FeatureDetector(shape){
    switch (shape)
    {
    case SQUARE:
      _hogTest.win_size = cv::Size(128, 128);
      break;
    case RECTANGLE:
      _hogTest.win_size = cv::Size(128, 64);
      break;
    }
  }
  void setTestHogFromHyperplane(std::vector<float>* hyperplane);
  std::map<std::string, std::vector<cv::Rect> > detectMultiScale(std::vector<std::string>& images);

private:
  cv::gpu::HOGDescriptor _hogTest;
};

#endif