#include "directory_parser.h"
#include "feature_detector.h"
#include "svm_binder.h"
#include <vector>

int main(int argc, char const *argv[])
{
	string posDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/pos/";
	string negDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/neg/";
	string testDirName="/home/igor/Work/Thesis/CarData/Freiburg/";

	vector<string> validExtensions;
    validExtensions.push_back("jpg");
    validExtensions.push_back("png");
    validExtensions.push_back("ppm");

	std::vector<string> positiveExsamples;
	std::vector<string> negativeExamples;
	DirectoryParser directoryParser;
	positiveExsamples = directoryParser.getFileNames(posDirName, validExtensions);
	negativeExamples = directoryParser.getFileNames(negDirName, validExtensions);

	FeatureDetector featureDetector;
	featureDetector.detectFeatures(positiveExsamples, FeatureDetector::POSITIVE);
	featureDetector.detectFeatures(negativeExamples, FeatureDetector::NEGATIVE);

	SvmBinder svmBinder;
	svmBinder.train(
		featureDetector.getFeatures(FeatureDetector::POSITIVE), 
		featureDetector.getFeatures(FeatureDetector::NEGATIVE));
	svmBinder.createHyperPlane();
	featureDetector.setTestHogFromHyperplane(svmBinder.getHyperPlane());

	std::vector<string> testExamples;
	testExamples = directoryParser.getFileNames(testDirName, validExtensions);
	featureDetector.detectMultiScale(testExamples);

	return 0;
}