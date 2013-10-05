#include "directory_parser.h"
#include "feature_detector.h"
#include <vector>

int main(int argc, char const *argv[])
{
	string posDirName="";
	string negDirName="";

	std::vector<string> positiveExsamples;
	std::vector<string> negativeExamples;
	DirectoryParser directoryParser;
	positiveExsamples = directoryParser.getFileNames(posDirName, validExtensions);
	negativeExamples = directoryParser.getFileNames(negDirName, validExtensions);

	FeatureDetector featureDetector;
	featureDetector.detectFeatures(positiveExsamples, FeatureDetector::POSITIVE);
	featureDetector.detectFeatures(negativeExamples, FeatureDetector::NEGATIVE)

	SvmBinder svmBiner;

	vector<int>* testWithHyperplane(
		featureDetector.getFeatures(FeatureDetector::POSITIVE), 
		featureDetector.getFeatures(FeatureDetector::NEGATIVE));
	std::vector<std::vector<float> >* getFeatures(FeatureEntity which);



	return 0;
}