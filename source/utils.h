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

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Utils
{
public:
  Utils();
  ~Utils();
  static double getMedian(std::vector<double> vals)
  {
    std::sort(vals.begin(), vals.end());
    double median = -1;
    int size = vals.size();
    if (size<=0) return -1;
    int mult = 1;
    int denom = 2;
    if (mult * size % denom == 0)
    {
        median = (vals[mult * size / denom - 1] + vals[mult * size / denom]) / denom;
    }
    else
    {
        median = vals[mult * size / denom];
    }
    return median;
  }

  static std::string parseImageName(const std::string& line)
  {
    int first_pos,end_pos;
    end_pos = line.length();
    first_pos = line.find(' ') + 1;
    return line.substr(first_pos, end_pos-first_pos);
  }

  static std::vector<std::string> splitString(std::string line, const std::string& delimiter)
  {
    size_t pos = 0;
    size_t curr = 0;
    std::vector<std::string> result;
    std::string token;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        result.push_back(token);
    }
    result.push_back(line);
    return result;
  }
};