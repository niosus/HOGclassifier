#include "temp_result_writer.h"
#include "highgui.h"
#include "logger.h"

ResultWriter::ResultWriter(std::string fileName)
{
  if (fileName == "")
  {
    fileName = "output_log.dat";
  }
  _file.open(fileName.c_str());
}

ResultWriter::~ResultWriter()
{
  _file.close();
}

void ResultWriter::showDetections()
{
  for (auto pairNameRect: _detectedCars)
  {
    std::string name = pairNameRect.first;
    cv::Mat image = cv::imread(name, CV_LOAD_IMAGE_COLOR);
    for (auto rect: pairNameRect.second)
    {
      cv::rectangle(image, rect.tl(), rect.br(), cv::Scalar(64, 255, 64), 3);
      this->addEntry(name, rect);
    }
    cv::imwrite((name+"___det.jpg").c_str(), image);
    Logger::instance()->logInfo("image written: ", name);
  }
}

void ResultWriter::storeDetections(const Map& foundRectsWithNames)
{
  for (auto entry : foundRectsWithNames)
  {
    _detectedCars[entry.first].insert(_detectedCars[entry.first].end(), entry.second.begin(), entry.second.end());
  }
}

void ResultWriter::addEntry(
    const std::string &imageName,
    const cv::Rect &rect)
{
  _file<<"IMAGE_NAME\t"<<imageName<<"\t";
  _file<<"RECT_COORDS\t"<<rect.x<<"\t"<<rect.y<<"\t";
  _file<<"RECT_SIZE\t"<<rect.width<<"\t"<<rect.height<<std::endl;
}