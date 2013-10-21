#ifndef CAR_DETECTOR_H
#define CAR_DETECTOR_H

#include <vector>
#include <map>
#include "cv.h"
#include "feature_detector.h"

class CarDetector
{
public:
  CarDetector();
  CarDetector(
    const std::vector<std::string> &posSampleNames,
    const std::vector<std::string> &negSampleNames,
    const std::vector<std::string> &testSampleNames,
    const FeatureDetector::SampleShape sampleShape);
  ~CarDetector();

  void detectCars();
  std::map<std::string, std::vector<cv::Rect> > getDetectedCarRects() const;
private:
  FeatureDetector *_featureDetector;

  std::vector<std::string> _posSampleNames;
  std::vector<std::string> _negSampleNames;
  std::vector<std::string> _testSampleNames;

  std::map<std::string, std::vector<cv::Rect> > _detectedCars;
};

#endif