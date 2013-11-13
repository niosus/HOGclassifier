#ifndef RESULT_WRITER
#define RESULT_WRITER

#include <iostream>
#include <fstream>
#include <unordered_map>
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
    const std::vector<float>& coords);
  void showDetections(
    const std::unordered_map<std::string, std::string> &leftRightNamesMap,
    const std::string &resultFolderName);
  void storeDetections(const Map& foundRectsWithNames);
private:
  std::ofstream _file;
  Map _detectedCars;
};

#endif