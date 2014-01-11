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

#ifndef BUMBLEBEE_CAMERA
#define BUMBLEBEE_CAMERA

class BumblebeeCameraParams
{
public:
  BumblebeeCameraParams()
  {
    this->fov = 105;
  }
  BumblebeeCameraParams(float focus, float baseline)
  {
    this->focus = focus;
    this->baseline = baseline;
    this->fov = 105;
  }
  float focus;
  float baseline;
  float fov;
};

#endif