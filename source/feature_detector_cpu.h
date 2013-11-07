#ifndef FEATURE_DETECTOR_CPU_H
#define FEATURE_DETECTOR_CPU_H

#include "feature_detector.h"
#include <vector>
#include <string>
#include <map>
#include "cv.h"

class FeatureDetectorCpu: public FeatureDetector
{
public:
  FeatureDetectorCpu(SampleShape shape): FeatureDetector(shape){
    switch (shape)
    {
    case SQUARE:
      _hogTest.winSize = cv::Size(128, 128);
      break;
    case RECTANGLE:
      _hogTest.winSize = cv::Size(128, 64);
      break;
    }
  }
  void setTestHogFromHyperplane(std::vector<float>* hyperplane);
  std::map<std::string, std::vector<cv::Rect> > detectMultiScale(std::vector<std::string>& images);

private:
  cv::HOGDescriptor _hogTest;
};

#endif