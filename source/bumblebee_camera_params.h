#ifndef BUMBLEBEE_CAMERA
#define BUMBLEBEE_CAMERA

class BumblebeeCameraParams
{
public:
  BumblebeeCameraParams(){}
  BumblebeeCameraParams(float focus, float baseline)
  {
    this->focus = focus;
    this->baseline = baseline;
  }
  float focus;
  float baseline;
};

#endif