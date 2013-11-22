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