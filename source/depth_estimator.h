#ifndef DEPTH_ESTIMATOR
#define DEPTH_ESTIMATOR

#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class DepthEstimator
{
public:
  DepthEstimator(){};
  void setExistingDepthImageNames(const std::vector<std::string> &imageNames);
  float getDepthMedian(const std::string& colorImageName, const cv::Rect& rect) const;
  std::string getDepthImageName(const std::string &colorImageName) const;
  ~DepthEstimator(){};

private:
  std::vector<std::string> _depthImageNames;
};

#endif //DEPTH_ESTIMATOR