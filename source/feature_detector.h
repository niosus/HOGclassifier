#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include "cv.h"
#include <vector>

class FeatureDetector
{
public:
	enum FeatureEntity {POSITIVE, NEGATIVE, TEST};

	FeatureDetector(){
		_hogTrain.winSize = cv::Size(128, 64);
		_hogTest.winSize = cv::Size(128, 64);
	};
	void detectFeatures(const std::vector<std::string>& filenames, FeatureEntity which);
	void setHogFromSvm(std::vector<float>* features);
	void setTestHogFromHyperplane(std::vector<double>* hyperplane);
	void detectMultiScale(std::vector<std::string>& images);
	std::vector<std::vector<float> >* getFeatures(FeatureEntity which);

private:
	std::vector<std::vector<float> > _featuresPos;
	std::vector<std::vector<float> > _featuresNeg;
	std::vector<std::vector<float> > _featuresTest;

	cv::HOGDescriptor _hogTrain;
	cv::HOGDescriptor _hogTest;

	void showDetections(const std::vector<cv::Rect>& found, cv::Mat& imageData);
};

#endif