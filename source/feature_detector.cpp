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

#include "feature_detector.h"
#include "temp_result_writer.h"
#include "highgui.h"
#include "logger.h"

/**
*
* This function refers to GitHub user DaHoC, so doesn't fall under the above licence,
* please refer to his trainHOG project for current licence.
*
**/
void FeatureDetector::detectFeatures(const std::vector<std::string>& filenames, FeatureEntity which)
{
	static const cv::Size trainingPadding = cv::Size(0, 0);
	static const cv::Size winStride = cv::Size(8, 8);
	std::vector<float> featureVector;
  Logger::instance()->logInfo("Detecting Features ");
  int tenPercentChunk = filenames.size()/10;
	for (int i=0; i< filenames.size(); ++i)
	{
    if (i%tenPercentChunk==0)
      Logger::instance()->logInfo("done percent", (i/tenPercentChunk)*10);

		cv::Mat imageData = cv::imread(filenames[i], CV_LOAD_IMAGE_COLOR);
    if (imageData.empty()) {
        featureVector.clear();
        printf("Error: HOG image '%s' is empty, features calculation skipped!\n", filenames[i].c_str());
        continue;
    }
    // Check for mismatching dimensions
    if (imageData.cols != _hogTrain.winSize.width || imageData.rows != _hogTrain.winSize.height) {
        featureVector.clear();
        printf(
        	"Error: Image '%s' dimensions (%u x %u) do not match HOG window size (%u x %u)!\n",
        	filenames[i].c_str(),
        	imageData.cols,
        	imageData.rows,
        	_hogTrain.winSize.width,
        	_hogTrain.winSize.height);
        continue;
    }
    std::vector<cv::Point> locations;
    _hogTrain.compute(imageData, featureVector, winStride, trainingPadding, locations);
    imageData.release(); // Release the image again after features are extracted
    switch (which)
		{
			case POSITIVE:
				_featuresPos.push_back(featureVector);
				break;
			case NEGATIVE:
				_featuresNeg.push_back(featureVector);
				break;
			case TEST:
				_featuresTest.push_back(featureVector);
				break;
		}
	}
  Logger::instance()->logInfo("Features Detected");
}

std::vector<std::vector<float> >* FeatureDetector::getFeatures(FeatureEntity which)
{
	switch (which)
	{
		case POSITIVE:
			return &_featuresPos;
		case NEGATIVE:
			return &_featuresNeg;
		case TEST:
			return &_featuresTest;
		default:
			return NULL;
	}
}
