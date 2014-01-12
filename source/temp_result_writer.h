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

#ifndef RESULT_WRITER
#define RESULT_WRITER

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "stdio.h"
#include "cv.h"
#include <fstream>
#include "depth_estimator.h"
#include "lasers_parser.h"

typedef std::map<std::string, std::vector<cv::Rect> > Map;

class ResultWriter
{
public:
  ResultWriter(
  const std::vector<std::string>& allImageNames,
  std::string fileName = "");
  ~ResultWriter();
  void addEntry(
    const std::string &imageName,
    const std::vector<double>& coords);
  void addEntry(
    const std::string &imageName,
    const double& x,
    const double& y);
  void showDetections(
    const std::unordered_map<std::string, std::string> &leftRightNamesMap,
    const std::string &resultFolderName);
  void showDetectionsLaser(
  const std::unordered_map<std::string, std::string> &leftRightNamesMap,
  const std::string &resultFolderName,
  LaserParser& laserParser);
  void storeDetections(const Map& foundRectsWithNames);
private:
  std::ofstream _file;
  Map _detectedCars;
  std::vector<std::string> _imagesWithNoCars;
};

#endif