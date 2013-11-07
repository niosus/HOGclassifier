#include "car_detector.h"
#include "logger.h"
#include "svm_binder.h"

const bool CarDetector::OVERWRITE_MODEL=true;
const bool CarDetector::LEAVE_OLD_MODEL=false;

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
  _featureDetector = new FeatureDetectorCpu(shape);
}

CarDetector::~CarDetector()
{
  delete _featureDetector;
  _featureDetector = nullptr;
}

void CarDetector::detectCars(const std::string& modelPath, bool overwriteModel)
{
  if (!_featureDetector)
  {
    //throw error
    return;
  }
  SvmBinder svmBinder;
  if (!svmBinder.loadModel(modelPath) || overwriteModel)
  {
    _featureDetector->detectFeatures(_posSampleNames, FeatureDetector::POSITIVE);
    _featureDetector->detectFeatures(_negSampleNames, FeatureDetector::NEGATIVE);
    Logger::instance()->logInfo("Starting Training");
    svmBinder.train(
      _featureDetector->getFeatures(FeatureDetector::POSITIVE),
      _featureDetector->getFeatures(FeatureDetector::NEGATIVE));
    svmBinder.saveModel(modelPath);
  }
  svmBinder.createDetectionVector();
  _featureDetector->setTestHogFromHyperplane(svmBinder.getDetectionVector());
  _detectedCars = _featureDetector->detectMultiScale(_testSampleNames);
}

std::map<std::string, std::vector<cv::Rect> > CarDetector::getDetectedCarRects() const
{
  return _detectedCars;
}