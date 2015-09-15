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

#define GNUPLOT "gnuplot -persist"

#include "temp_result_writer.h"
#include "highgui.h"
#include "logger.h"
#include "utils.h"

ResultWriter::ResultWriter(
  const std::vector<std::string>& allImageNames,
  std::string fileName) {
  file.open("temp.dat", std::ofstream::out);
  if (fileName == "") {
    fileName = "output_log.dat";
  }
  _file.open(fileName.c_str());
  // put all image names as we also need to keep track
  // of the image with no cars detected
  _file << "TOTAL_NUM_IMAGES\t\t" << allImageNames.size() << endl;
  for (const auto& imageName : allImageNames) {
    auto found = imageName.find_last_of("/\\");
    _file << imageName.substr(found + 1) << std::endl;
  }
}

ResultWriter::~ResultWriter() {
  _file.close();
  file.close();
}

void ResultWriter::showDetections(
  const std::string &resultFolderName) {
  Logger::instance()->logInfo("Showing detections");
  for (auto pairNameRect : _detectedCars) {
    std::string imageLeftName = pairNameRect.first;
    Logger::instance()->logInfo("imageLeftName", imageLeftName);
    cv::Mat image = cv::imread(imageLeftName, CV_LOAD_IMAGE_COLOR);
    std::string realImageName = Utils::parseImageName(imageLeftName);
    cv::imwrite((resultFolderName + realImageName).c_str(), image);
    for (auto rect : pairNameRect.second) {
      if (rect.width > 0 && rect.height > 0) {
        cv::rectangle(image, rect.tl(), rect.br(), cv::Scalar(64, 255, 64), 3);
      }
    }
    cv::imwrite((resultFolderName + realImageName + "___det.jpg").c_str(), image);
    Logger::instance()->logInfo("image written: ", realImageName);
  }
}

// void ResultWriter::showDetectionsLaser(
//   const std::unordered_map<std::string, std::string> &leftRightNamesMap,
//   const std::string &resultFolderName,
//   LaserParser& laserParser) {
//   Logger::instance()->logInfo("Showing detections");
//   double fullFoV = 97;
//   for (auto pairNameRect : _detectedCars) {
//     std::string imageLeftName = pairNameRect.first;
//     Logger::instance()->logInfo("imageLeftName", imageLeftName);
//     cv::Mat image = cv::imread(imageLeftName, CV_LOAD_IMAGE_COLOR);
//     std::string realImageName = Utils::parseImageName(imageLeftName);
//     cv::imwrite((resultFolderName + realImageName).c_str(), image);
//     for (auto rect : pairNameRect.second) {
//       if (rect.width > 0 && rect.height > 0) {
//         cv::rectangle(image, rect.tl(), rect.br(), cv::Scalar(64, 255, 64), 3);
//         double fovStart = ((double)rect.x / image.cols) * fullFoV - fullFoV / 2; //97 is the camera fov TODO hack
//         double fovEnd = (((double)rect.x + rect.width) / image.cols) * fullFoV - fullFoV / 2; //97 is the camera fov TODO hack
//         std::cout << "fields of view " << "start:" << fovStart << " end:" << fovEnd << std::endl;
//         float angleOfCamera = 90;
//         double temp;
//         temp = fovEnd;
//         fovEnd = -fovStart + angleOfCamera;
//         fovStart = - temp + angleOfCamera; // hack the camera is rotated by 90 degrees
//         fovStart = (fovStart / 180) * M_PI;
//         fovEnd = (fovEnd / 180) * M_PI;
//         std::vector<double> xVec;
//         std::vector<double> yVec;
//         double imageX, imageY;
//         laserParser.getImagePos(realImageName, imageX, imageY);
//         laserParser.getPointsForImageFov(realImageName, fovStart, fovEnd, xVec, yVec);
//         std::cout << "sizes of vectors with points " << xVec.size() << " " << yVec.size() << std::endl;
//         double medianX = Utils::getMedian(xVec);
//         double medianY = Utils::getMedian(yVec);
//         Logger::instance()->logInfo("median x", medianX);
//         Logger::instance()->logInfo("median y", medianY);
//         this->addEntry(imageLeftName, medianX, medianY);
//         plot(imageX, imageY);
//         plot(xVec[0], yVec[0]);
//         plot(xVec[xVec.size() - 1], yVec[yVec.size() - 1]);
//         plot(imageX, imageY);
//       }
//     }
//     cv::imwrite((resultFolderName + realImageName + "___det.jpg").c_str(), image);
//     Logger::instance()->logInfo("image written: ", realImageName);
//   }
// }

void ResultWriter::storeDetections(const Map& foundRectsWithNames) {
  for (auto entry : foundRectsWithNames) {
    _detectedCars[entry.first].insert(_detectedCars[entry.first].end(), entry.second.begin(), entry.second.end());
  }
}

void ResultWriter::plot(
  const std::vector<double>& xVec,
  const std::vector<double>& yVec) {
  for (int i = 0; i < xVec.size(); ++i) {
    file << std::fixed << std::setprecision(3);
    file << "X\t" << xVec[i] << "\tY\t" << yVec[i] << endl;
  }
  FILE *gp;
  gp = popen(GNUPLOT, "w"); /* 'gp' is the pipe descriptor */
  if (gp == NULL) {
    printf("Error opening pipe to GNU plot. Check if you have it! \n");
    return;
  }
  fprintf(gp, "plot \"./temp.dat\" using 2:4");
  fclose(gp);
}

void ResultWriter::plot(
  const double& x,
  const double& y) {
  file << std::fixed << std::setprecision(3);
  file << "X\t" << x << "\tY\t" << y << endl;
}

void ResultWriter::addEntry(
  const std::string &imageName,
  const std::vector<double>& coords) {
  auto found = imageName.find_last_of("/\\");
  _file << "IMAGE_NAME:" << imageName.substr(found + 1) << "\t";
  _file << "X:" << coords[0] << "\tY:" << coords[1] << "\tZ:" << coords[2] << endl;
}

void ResultWriter::addEntry(
  const std::string &imageName,
  const double& x,
  const double& y) {
  if (x < 0 || y < 0) return;
  _file << std::fixed << std::setprecision(3);
  _file << "IMAGE_NAME:" << Utils::parseImageName(imageName) << "\t";
  _file << "X\t" << x << "\tY\t" << y << endl;
}