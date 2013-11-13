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

void ResultWriter::showDetections(
  const std::unordered_map<std::string, std::string> &leftRightNamesMap,
  const std::string &resultFolderName)
{
  Logger::instance()->logInfo("Showing detections");
  DepthEstimator depthEstimator;
  for (auto pairNameRect: _detectedCars)
  {
    std::string imageLeftName = pairNameRect.first;
    std::string imageRightName = leftRightNamesMap.find(imageLeftName)->second;
    Logger::instance()->logInfo("imageLeftName", imageLeftName);
    Logger::instance()->logInfo("imageRightName", imageRightName);
    depthEstimator.setCurrentDepthMapFromImage(imageLeftName, imageRightName);
    cv::Mat image = cv::imread(imageLeftName, CV_LOAD_IMAGE_COLOR);
    size_t lastSlashPos = imageLeftName.find_last_of("/");
    std::string realImageName = imageLeftName.substr(lastSlashPos+1);
    cv::imwrite((resultFolderName + realImageName).c_str(), image);
    for (auto rect: pairNameRect.second)
    {
      if (rect.width > 0 && rect.height > 0)
      {
        cv::rectangle(image, rect.tl(), rect.br(), cv::Scalar(64, 255, 64), 3);
        std::vector<float> median = depthEstimator.getDepthMedian(rect);
        Logger::instance()->logInfo("median x", median[0]);
        Logger::instance()->logInfo("median y", median[1]);
        Logger::instance()->logInfo("median z", median[2]);
        this->addEntry(imageLeftName, median);
      }
    }
    cv::imwrite((resultFolderName + realImageName + "___det.jpg").c_str(), image);
    Logger::instance()->logInfo("image written: ", realImageName);
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
    const std::vector<float>& coords)
{
  auto found = imageName.find_last_of("/\\");
  _file<<"IMAGE_NAME:"<<imageName.substr(found+1)<<"\t";
  _file<<"X:"<<coords[0]<<"\tY:"<<coords[1]<<"\tZ:"<<coords[2]<<endl;
}