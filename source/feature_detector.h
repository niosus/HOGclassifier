#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include "cv.h"
#include <vector>
#include <string>
#include <map>

class FeatureDetector
{
public:
	enum FeatureEntity {POSITIVE, NEGATIVE, TEST};
	enum SampleShape {SQUARE, RECTANGLE};

	FeatureDetector(SampleShape shape){
		switch (shape)
		{
		case SQUARE:
			_hogTest.winSize = cv::Size(128, 128);
			_hogTrain.winSize = cv::Size(128, 128);
			break;
		case RECTANGLE:
			_hogTest.winSize = cv::Size(128, 64);
			_hogTrain.winSize = cv::Size(128, 64);
			break;
		}

	};
	void detectFeatures(const std::vector<std::string>& filenames, FeatureEntity which);
	void setHogFromSvm(std::vector<float>* features);
	void setTestHogFromHyperplane(std::vector<double>* hyperplane);
	std::map<std::string, std::vector<cv::Rect> > detectMultiScale(std::vector<std::string>& images);
	std::vector<std::vector<float> >* getFeatures(FeatureEntity which);

private:
	std::vector<std::vector<float> > _featuresPos;
	std::vector<std::vector<float> > _featuresNeg;
	std::vector<std::vector<float> > _featuresTest;

	cv::HOGDescriptor _hogTest;
	cv::HOGDescriptor _hogTrain;
};

#endif