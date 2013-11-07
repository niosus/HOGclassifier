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
			_hogTrain.winSize = cv::Size(128, 128);
			break;
		case RECTANGLE:
			_hogTrain.winSize = cv::Size(128, 64);
			break;
		}
	}
	void detectFeatures(const std::vector<std::string>& filenames, FeatureEntity which);
	void setHogFromSvm(std::vector<float>* features);
	std::vector<std::vector<float> >* getFeatures(FeatureEntity which);
	virtual void setTestHogFromHyperplane(std::vector<float>* hyperplane) = 0;
	virtual std::map<std::string, std::vector<cv::Rect> > detectMultiScale(std::vector<std::string>& images) = 0;

protected:
	std::vector<std::vector<float> > _featuresPos;
	std::vector<std::vector<float> > _featuresNeg;
	std::vector<std::vector<float> > _featuresTest;

	cv::HOGDescriptor _hogTrain;
};

#endif