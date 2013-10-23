#include "car_detector.h"
#include "logger.h"
#include "svm_binder.h"

CarDetector::CarDetector(){}

CarDetector::CarDetector(
    const std::vector<std::string> &posSampleNames,
    const std::vector<std::string> &negSampleNames,
    const std::vector<std::string> &testSampleNames,
    const FeatureDetector::SampleShape shape)
{
  _posSampleNames = posSampleNames;
  _negSampleNames = negSampleNames;
  _testSampleNames = testSampleNames;
  _featureDetector = new FeatureDetector(shape);
}

CarDetector::~CarDetector()
{
  delete _featureDetector;
  _featureDetector = nullptr;
}

void CarDetector::detectCars()
{
  if (!_featureDetector)
  {
    //throw error
    return;
  }
  _featureDetector->detectFeatures(_posSampleNames, FeatureDetector::POSITIVE);
  _featureDetector->detectFeatures(_negSampleNames, FeatureDetector::NEGATIVE);
  Logger::instance()->logInfo("Starting Training");
  SvmBinder svmBinder;
  svmBinder.train(
    _featureDetector->getFeatures(FeatureDetector::POSITIVE),
    _featureDetector->getFeatures(FeatureDetector::NEGATIVE));
  svmBinder.createDetectionVector();

  _featureDetector->setTestHogFromHyperplane(svmBinder.getDetectionVector());
  _detectedCars = _featureDetector->detectMultiScaleCpu(_testSampleNames);
}

std::map<std::string, std::vector<cv::Rect> > CarDetector::getDetectedCarRects() const
{
  return _detectedCars;
}