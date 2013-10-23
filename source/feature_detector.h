#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include <vector>
#include <string>
#include <map>
#include "cv.h"
#include "opencv2/gpu/gpu.hpp"

class FeatureDetector
{
public:
	enum FeatureEntity {POSITIVE, NEGATIVE, TEST};
	enum SampleShape {SQUARE, RECTANGLE};

	FeatureDetector(SampleShape shape){
		switch (shape)
		{
		case SQUARE:
			_hogTestGpu.win_size = cv::Size(128, 128);
			_hogTestCpu.winSize = cv::Size(128, 128);
			_hogTrain.winSize = cv::Size(128, 128);
			break;
		case RECTANGLE:
			_hogTestGpu.win_size = cv::Size(128, 64);
			_hogTestCpu.winSize = cv::Size(128, 64);
			_hogTrain.winSize = cv::Size(128, 64);
			break;
		}

	};
	void detectFeatures(const std::vector<std::string>& filenames, FeatureEntity which);
	void setHogFromSvm(std::vector<float>* features);
	void setTestHogFromHyperplane(std::vector<float>* hyperplane);
	std::map<std::string, std::vector<cv::Rect> > detectMultiScaleGpu(std::vector<std::string>& images);
	std::map<std::string, std::vector<cv::Rect> > detectMultiScaleCpu(std::vector<std::string>& images);
	std::vector<std::vector<float> >* getFeatures(FeatureEntity which);

private:
	std::vector<std::vector<float> > _featuresPos;
	std::vector<std::vector<float> > _featuresNeg;
	std::vector<std::vector<float> > _featuresTest;

	cv::gpu::HOGDescriptor _hogTestGpu;
	cv::HOGDescriptor _hogTestCpu;
	cv::HOGDescriptor _hogTrain;
};

#endif