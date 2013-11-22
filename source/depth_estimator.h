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

#ifndef DEPTH_ESTIMATOR
#define DEPTH_ESTIMATOR

#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "bumblebee_camera_params.h"

class DepthEstimator
{
public:
  DepthEstimator();
  void setCurrentDepthMapFromImage(
    const std::string &imageLeftName,
    const std::string &imageRightName);
  std::vector<float> getDepthMedian(
    const cv::Rect &rect) const;
  cv::Mat getDisparity(
    const cv::Mat &imageLeft,
    const cv::Mat &imageRight);
  void getDepthFromDisparity(
    const cv::Mat &disparity,
    const BumblebeeCameraParams &cameraParams,
    cv::Mat& result);

private:
  cv::Mat _depthMap; // an opencv image of type CV_32FC3
};

#endif //DEPTH_ESTIMATOR