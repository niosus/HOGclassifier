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