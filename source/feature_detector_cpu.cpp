#include "feature_detector_cpu.h"
#include "temp_result_writer.h"
#include "highgui.h"
#include "logger.h"

void FeatureDetectorCpu::setTestHogFromHyperplane(std::vector<float>* hyperplane)
{
  _hogTest.setSVMDetector(*hyperplane);
}

std::map<std::string, std::vector<cv::Rect> > FeatureDetectorCpu::detectMultiScale(std::vector<std::string>& imageNames)
{
  std::map<std::string, std::vector<cv::Rect> > allFoundRects;
  std::vector<cv::Rect> found;
  int groupThreshold = 2;
  cv::Size padding(cv::Size(16, 16));
  cv::Size winStride(cv::Size(8, 8));
  double hitThreshold = 0.0; // tolerance
  int size = imageNames.size();
  int tenPercentChunk = size / 10;
  int count = 0;
  Logger::instance()->logInfo("Counting features multiscale...");
  for (auto imageName: imageNames)
  {
    if (tenPercentChunk != 0 && count % tenPercentChunk == 0)
    {
      Logger::instance()->logInfo("Percent done = ", (count / tenPercentChunk) * 10);
    }
    cv::Mat testImage = cv::imread(imageName.c_str(), CV_LOAD_IMAGE_COLOR);
    _hogTest.detectMultiScale(testImage, found, hitThreshold, winStride, padding, 1.05, groupThreshold);
    for (cv::Rect rect: found)
    {
      allFoundRects[imageName].push_back(rect);
    }
    count++;
  }
  Logger::instance()->logInfo("Done...");
  return allFoundRects;
}
