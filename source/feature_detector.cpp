#include "feature_detector.h"

void FeatureDetector::detectFeatures(const std::vector<string>& filenames, FeatureEntity which)
{
	HOGDescriptor hog;
	hog.winSize = Size(128, 64); 
	static const Size trainingPadding = Size(0, 0);
	static const Size winStride = Size(8, 8);
	std::vector<float> featureVector;
	for (int i=0; i< filenames.size(); ++i)
	{
		Mat imageData = imread(imageFilename, 0);
	    if (imageData.empty()) {
	        featureVector.clear();
	        printf("Error: HOG image '%s' is empty, features calculation skipped!\n", imageFilename.c_str());
	        continue;
	    }
	    // Check for mismatching dimensions
	    if (imageData.cols != hog.winSize.width || imageData.rows != hog.winSize.height) {
	        featureVector.clear();
	        printf("Error: Image '%s' dimensions (%u x %u) do not match HOG window size (%u x %u)!\n", imageFilename.c_str(), imageData.cols, imageData.rows, hog.winSize.width, hog.winSize.height);
	        continue;
	    }
	    vector<Point> locations;
	    hog.compute(imageData, featureVector, winStride, trainingPadding, locations);
	    imageData.release(); // Release the image again after features are extracted
	    switch (which)
		{
			case FeatureEntity::POSITIVE:
				_featuresPos.push_back(featureVector);
				break;
			case FeatureEntity::NEGATIVE:
				_featuresNeg.push_back(featureVector);
				break;
		}
	}
}

std::vector<std::vector<float> >* FeatureDetector::getFeatures(FeatureEntity which)
{
	switch (which)
	{
		case FeatureEntity::POSITIVE:
			return &_featuresPos;
		case FeatureEntity::NEGATIVE:
			return &_featuresNeg;
		default:
			return NULL;
	}
}
