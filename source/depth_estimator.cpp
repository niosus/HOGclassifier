#include "depth_estimator.h"
#include <cassert>
#include <iostream>

void DepthEstimator::setExistingDepthImageNames(const std::vector<std::string> &imageNames)
{
  _depthImageNames = imageNames;
}

std::string DepthEstimator::getDepthImageName(const std::string &colorImageName) const
{
  auto foundNumberBegin = colorImageName.find_last_of("_");
  auto foundNumberEnd = colorImageName.find_last_of(".");
  foundNumberBegin++;
  std::string numberString = colorImageName.substr(foundNumberBegin, foundNumberEnd - foundNumberBegin);
  int num = std::stoi(numberString);
  return _depthImageNames[num];
}

float DepthEstimator::getDepthMedian(const std::string& colorImageName, const cv::Rect& rect) const
{
  std::string depthImageName = getDepthImageName(colorImageName);
  cv::Mat image = cv::imread(depthImageName, CV_LOAD_IMAGE_UNCHANGED);
  image.convertTo(image, CV_32F);
  assert(image.depth() == CV_32F);
  cv::Mat roiImage = cv::Mat(image, rect);
  assert(roiImage.depth() == CV_32F);
  std::vector<float> pixels;
  pixels.reserve(roiImage.total());
  for (int r = 0; r < roiImage.rows; ++r)
  {
    for (int c = 0; c < roiImage.cols; ++c)
    {
      float pixel = image.at<float>(r, c);
      if (pixel > 0)
        std::cout<<pixel<<std::endl;
      pixels.push_back(pixel);
    }
  }
  return 0;
}
