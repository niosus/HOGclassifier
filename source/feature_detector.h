#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include "opencv.h"

class FeatureDetector
{
public:
	enum FeatureEntity {POSITIVE, NEGATIVE};

	FeatureDetector(){};
	void detectFeatures(const std::vector<string>& filenames, FeatureEntity which);
	std::vector<std::vector<float> >* getFeatures(FeatureEntity which);

private:
	std::vector<std::vector<float> > _featuresPos;
	std::vector<std::vector<float> > _featuresNeg;
};

#endif