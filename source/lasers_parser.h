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

#ifndef LASER_PARSER_H
#define LASER_PARSER_H

#include <vector>
#include <unordered_map>
#include <string>

class LaserParser
{
public:
  LaserParser(const std::string& fileName);
  void getPointsForImageFov(
    const std::string& imageName,
    const double& fovStart,
    const double& fovEnd,
    std::vector<double>& xVec,
    std::vector<double>& yVec
    );

private:
  std::unordered_map<std::string, std::vector<double> > _angles;
  std::unordered_map<std::string, std::vector<double> > _pointsX;
  std::unordered_map<std::string, std::vector<double> > _pointsY;
};

#endif