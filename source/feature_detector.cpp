#include "feature_detector.h"
#include "temp_result_writer.h"
#include "highgui.h"
#include "logger.h"

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
