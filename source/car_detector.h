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