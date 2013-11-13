#include "depth_estimator.h"
#include <cassert>
#include <iostream>
#include "opencv2/calib3d/calib3d.hpp"

DepthEstimator::DepthEstimator()
{
}

void DepthEstimator::setCurrentDepthMapFromImage(
  const std::string &imageLeftName,
  const std::string &imageRightName)
{
  BumblebeeCameraParams bumblebeeParams;
  bumblebeeParams.focus = 0.572435f;
  bumblebeeParams.baseline = 0.119188f;
  cv::Mat imgLeft = cv::imread(imageLeftName, CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat imgRight = cv::imread(imageRightName, CV_LOAD_IMAGE_GRAYSCALE);
  // init depth map
  _depthMap = cv::Mat(imgLeft.rows, imgRight.cols, CV_32FC3);
  getDepthFromDisparity(
    getDisparity(imgLeft, imgRight),
    bumblebeeParams,
    _depthMap);
}

float getMedian(std::vector<float> vals)
{
  std::sort(vals.begin(), vals.end());
  float median = -1;
  int size = vals.size();
  if (size<=0) return -1;
  if (size % 2 == 0)
  {
      median = (vals[size / 2 - 1] + vals[size / 2]) / 2;
  }
  else
  {
      median = vals[size / 2];
  }
  return median;
}

std::vector<float> DepthEstimator::getDepthMedian(
  const cv::Rect &rect) const
{
  cv::Rect newRect = cv::Rect(
    std::max(rect.x, 0),
    std::max(rect.y, 0),
    rect.width,
    rect.height);
  cv::Mat roiImage = cv::Mat(_depthMap, newRect);
  assert(roiImage.depth() == CV_32F);
  assert(roiImage.channels() == 3);
  std::vector<float> xVals;
  std::vector<float> yVals;
  std::vector<float> zVals;
  xVals.reserve(roiImage.total());
  yVals.reserve(roiImage.total());
  zVals.reserve(roiImage.total());
  for (int r = 0; r < roiImage.rows; ++r)
  {
    for (int c = 0; c < roiImage.cols; ++c)
    {
      cv::Vec3f pixel = roiImage.at<cv::Vec3f>(r, c);
      // std::cout<<"getDepthMedian"<<std::endl;
      // std::cout<<"r="<<r<<" c="<<c<<std::endl;
      if (pixel(2)!=0)
      {
        xVals.push_back(pixel(0));
        yVals.push_back(pixel(1));
        zVals.push_back(pixel(2));
      }
    }
  }
  std::vector<float> result;
  result.push_back(getMedian(xVals));
  result.push_back(getMedian(yVals));
  result.push_back(getMedian(zVals));
  return result;
}

cv::Mat DepthEstimator::getDisparity(
  const cv::Mat &imageLeft,
  const cv::Mat &imageRight)
{
  int ndisparities = 64;   /* Range of disparity */
  int SADWindowSize = 21; /* Size of the block window. Must be odd */
  cv::Mat imageDisparity = cv::Mat(imageLeft.rows, imageLeft.cols, CV_16S);
  cv::StereoBM sbm( cv::StereoBM::FISH_EYE_PRESET,
                  ndisparities,
                  SADWindowSize );
  //-- 3. Calculate the disparity image
  sbm( imageLeft, imageRight, imageDisparity, CV_16S );
  return imageDisparity;
}

void getCentralColAndRow(const int& rows, const int& cols,
  int &centralRow, int& centralCol)
{
  centralCol = cols/2;
  centralRow = rows/2;
}

/**
*
* @param disparity: an opencv Mat of type short (CV_16S) with disparity
* @param cameraParams: camera params - focus and baseline
* @param result: Mat of type float (CV_32F) holds distance to object in every pixel
*
**/
void DepthEstimator::getDepthFromDisparity(
  const cv::Mat &disparity,
  const BumblebeeCameraParams &cameraParams,
  cv::Mat& result)
{
  int centralCol, centralRow;
  assert(disparity.rows > 0 && disparity.cols > 0);
  getCentralColAndRow(disparity.rows, disparity.cols, centralRow, centralCol);
  for (int r = 0; r < disparity.rows; ++r)
  {
    for (int c = 0; c < disparity.cols; ++c)
    {
      short disparityShort = (short) disparity.at<short>(r, c);
      float tempDoubleDisparity = (float) disparityShort;
      if (tempDoubleDisparity > 0)
      {
        float depth = (cameraParams.focus * 1024 * cameraParams.baseline * 16) / tempDoubleDisparity;
        float y = ((r-centralRow) * depth) / (cameraParams.focus * 1024);
        float x = ((c-centralCol) * depth) / (cameraParams.focus * 1024);
        cv::Vec3f pixel(x,y,depth);
        result.at<cv::Vec3f>(r,c) = pixel;
      }
      else
      {
        cv::Vec3f pixel(0,0,0);
        result.at<cv::Vec3f>(r,c) = pixel;
      }
    }
  }
}
