// This file is part of HOGclassifier.

// HOGclassifier is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// HOGclassifier is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with HOGclassifier.  If not, see <http://www.gnu.org/licenses/>.


#include "car_detector.h"
#include "logger.h"
#include "svm_binder.h"

const bool CarDetector::OVERWRITE_MODEL = true;
const bool CarDetector::LEAVE_OLD_MODEL = false;

CarDetector::CarDetector()
{
    _featureDetector = nullptr;
}

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

void CarDetector::detectCars(const std::string &modelPath, bool overwriteModel)
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
    return this->_detectedCars;
}
