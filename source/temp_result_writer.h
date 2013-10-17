#ifndef RESULT_WRITER
#define RESULT_WRITER

#include <iostream>
#include <fstream>
#include "stdio.h"
#include "cv.h"
#include <fstream>

class ResultWriter
{
public:
  ResultWriter(std::string fileName="");
  ~ResultWriter();
  void addEntry(
    const std::string &imageName,
    const cv::Rect &rect);
  void showDetections(const std::vector<std::pair<std::string, cv::Rect> >& foundRectsWithNames);
private:
  std::ofstream _file;
};

#endif