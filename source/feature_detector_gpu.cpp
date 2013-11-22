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

#include "feature_detector_gpu.h"
#include "temp_result_writer.h"
#include "highgui.h"
#include "logger.h"

void FeatureDetectorGpu::setTestHogFromHyperplane(std::vector<float>* hyperplane)
{
	_hogTest.setSVMDetector(*hyperplane);
}

std::map<std::string, std::vector<cv::Rect> > FeatureDetectorGpu::detectMultiScale(std::vector<std::string>& imageNames)
{
  std::map<std::string, std::vector<cv::Rect> > allFoundRects;
	std::vector<cv::Rect> found;
  int groupThreshold = 2;
  cv::Size padding(cv::Size(0, 0));
  cv::Size winStride(cv::Size(8, 8));
  double hitThreshold = 0.0; // tolerance
  cv::gpu::GpuMat testImage;
	for (auto imageName: imageNames)
	{
		cv::Mat testImage_cpu = cv::imread(imageName.c_str(), CV_LOAD_IMAGE_COLOR);
    cv::Mat testImage_cpu_rgba;
    cv::cvtColor(testImage_cpu, testImage_cpu_rgba, CV_BGR2BGRA);
    testImage.upload(testImage_cpu_rgba);
		_hogTest.detectMultiScale(testImage, found, hitThreshold, winStride, padding, 1.05, groupThreshold);
    for (cv::Rect rect: found)
    {
      allFoundRects[imageName].push_back(rect);
    }
    Logger::instance()->logInfo("computed hogs for", imageName);
	}
  return allFoundRects;
}