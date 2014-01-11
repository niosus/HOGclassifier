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

#include "depth_estimator.h"
#include <cassert>
#include <iostream>
#include "opencv2/calib3d/calib3d.hpp"
#include "utils.h"

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

std::vector<double> DepthEstimator::getDepthMedian(
  const cv::Rect &rect) const
{
  // update rect size in case it does not fin to the image
  int newX = std::max(rect.x, 0);
  int newY = std::max(rect.y, 0);
  int newWidth = std::min(rect.width, _depthMap.cols - newX);
  int newHeight = std::min(rect.height, _depthMap.rows - newY);
  cv::Rect newRect = cv::Rect(
    newX, newY, newWidth, newHeight);
  cv::Mat roiImage = cv::Mat(_depthMap, newRect);
  assert(roiImage.depth() == CV_32F);
  assert(roiImage.channels() == 3);
  std::vector<double> xVals;
  std::vector<double> yVals;
  std::vector<double> zVals;
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
  std::vector<double> result;
  result.push_back(Utils::getMedian(xVals));
  result.push_back(Utils::getMedian(yVals));
  result.push_back(Utils::getMedian(zVals));
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
* @param result: Mat of type double (CV_32F) holds distance to object in every pixel
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
      double tempDoubleDisparity = (double) disparityShort;
      if (tempDoubleDisparity > 0)
      {
        double depth = (cameraParams.focus * 1024 * cameraParams.baseline * 16) / tempDoubleDisparity;
        double y = ((r-centralRow) * depth) / (cameraParams.focus * 1024);
        double x = ((c-centralCol) * depth) / (cameraParams.focus * 1024);
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
