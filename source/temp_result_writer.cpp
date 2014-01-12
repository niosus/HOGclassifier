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

#include "temp_result_writer.h"
#include "highgui.h"
#include "logger.h"
#include "utils.h"

ResultWriter::ResultWriter(
  const std::vector<std::string>& allImageNames,
  std::string fileName)
{
  if (fileName == "")
  {
    fileName = "output_log.dat";
  }
  _file.open(fileName.c_str());
  // put all image names as we also need to keep track
  // of the image with no cars detected
  _file<<"TOTAL_NUM_IMAGES\t\t"<<allImageNames.size()<<endl;
  for (const auto& imageName: allImageNames)
  {
    auto found = imageName.find_last_of("/\\");
    _file<<imageName.substr(found+1)<<std::endl;
  }
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
        std::vector<double> median = depthEstimator.getDepthMedian(rect);
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

void ResultWriter::showDetectionsLaser(
  const std::unordered_map<std::string, std::string> &leftRightNamesMap,
  const std::string &resultFolderName,
  LaserParser& laserParser)
{
  Logger::instance()->logInfo("Showing detections");
  double fullFoV = 105;
  for (auto pairNameRect: _detectedCars)
  {
    std::string imageLeftName = pairNameRect.first;
    Logger::instance()->logInfo("imageLeftName", imageLeftName);
    cv::Mat image = cv::imread(imageLeftName, CV_LOAD_IMAGE_COLOR);
    std::string realImageName = Utils::parseImageName(imageLeftName);
    cv::imwrite((resultFolderName + realImageName).c_str(), image);
    for (auto rect: pairNameRect.second)
    {
      if (rect.width > 0 && rect.height > 0)
      {
        cv::rectangle(image, rect.tl(), rect.br(), cv::Scalar(64, 255, 64), 3);
        double fovStart = ((double)rect.x / image.rows) * fullFoV - fullFoV / 2; //105 is the camera fov TODO hack
        double fovEnd = (((double)rect.x + rect.width) / image.rows) * fullFoV - fullFoV / 2; //105 is the camera fov TODO hack
        std::cout<<"fields of view "<<"start:"<<fovStart<<" end:"<<fovEnd<<std::endl;
        if (fovStart < 0 || fovEnd < 0) continue;
        fovStart-=90;
        fovEnd-=90; // hack the camera is rotated by 90 degrees
        fovStart = (fovStart / 180) * M_PI;
        fovEnd = (fovEnd / 180) * M_PI;
        std::vector<double> xVec;
        std::vector<double> yVec;
        laserParser.getPointsForImageFov(realImageName, fovStart, fovEnd, xVec, yVec);
        std::cout<<"sizes of vectors with points "<<xVec.size()<<" "<<yVec.size()<<std::endl;
        double medianX = Utils::getMedian(xVec);
        double medianY = Utils::getMedian(yVec);
        Logger::instance()->logInfo("median x", medianX);
        Logger::instance()->logInfo("median y", medianY);
        this->addEntry(imageLeftName, medianX, medianY);
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
    const std::vector<double>& coords)
{
  auto found = imageName.find_last_of("/\\");
  _file<<"IMAGE_NAME:"<<imageName.substr(found+1)<<"\t";
  _file<<"X:"<<coords[0]<<"\tY:"<<coords[1]<<"\tZ:"<<coords[2]<<endl;
}

void ResultWriter::addEntry(
    const std::string &imageName,
    const double& x,
    const double& y)
{
  _file << std::fixed << std::setprecision(3);
  _file<<"IMAGE_NAME:"<<Utils::parseImageName(imageName)<<"\t";
  _file<<"X:"<<x<<"\tY:"<<y<<endl;
}