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
	virtual ~FeatureDetector(){}
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
