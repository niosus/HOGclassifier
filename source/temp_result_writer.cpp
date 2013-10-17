#include "temp_result_writer.h"
#include "highgui.h"

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

void ResultWriter::showDetections(const std::vector<std::pair<std::string, cv::Rect> >& foundRectsWithNames)
{
  for (auto pairNameRect: foundRectsWithNames)
  {
    cv::Mat image = cv::imread(pairNameRect.first, CV_LOAD_IMAGE_COLOR);
    cv::rectangle(image, pairNameRect.second.tl(), pairNameRect.second.br(), cv::Scalar(64, 255, 64), 3);
    this->addEntry(pairNameRect.first, pairNameRect.second);
    cv::imwrite((pairNameRect.first+"___det.jpg").c_str(), image);
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