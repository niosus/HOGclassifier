#ifndef RESULT_WRITER
#define RESULT_WRITER

#include <iostream>
#include <fstream>
#include "stdio.h"
#include "cv.h"
#include <fstream>
#include "depth_estimator.h"

typedef std::map<std::string, std::vector<cv::Rect> > Map;

class ResultWriter
{
public:
  ResultWriter(std::string fileName="");
  ~ResultWriter();
  void addEntry(
    const std::string &imageName,
    const cv::Rect &rect,
    const DepthEstimator &depthEstimator);
  void showDetections(const DepthEstimator &depthEstimator);
  void storeDetections(const Map& foundRectsWithNames);
private:
  std::ofstream _file;
  Map _detectedCars;
};

#endif