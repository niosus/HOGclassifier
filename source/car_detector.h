#ifndef CAR_DETECTOR_H
#define CAR_DETECTOR_H

#include <vector>
#include <map>
#include "cv.h"
#include "feature_detector_cpu.h"
#include "feature_detector_gpu.h"

class CarDetector
{
public:
  static const bool OVERWRITE_MODEL;
  static const bool LEAVE_OLD_MODEL;
  CarDetector();
  CarDetector(
    const std::vector<std::string> &posSampleNames,
    const std::vector<std::string> &negSampleNames,
    const std::vector<std::string> &testSampleNames,
    const FeatureDetector::SampleShape sampleShape);
  ~CarDetector();

  void detectCars(const std::string& modelPath, bool overwriteModel=LEAVE_OLD_MODEL);
  std::map<std::string, std::vector<cv::Rect> > getDetectedCarRects() const;
private:
  FeatureDetector *_featureDetector;

  std::vector<std::string> _posSampleNames;
  std::vector<std::string> _negSampleNames;
  std::vector<std::string> _testSampleNames;

  std::map<std::string, std::vector<cv::Rect> > _detectedCars;
};

#endif