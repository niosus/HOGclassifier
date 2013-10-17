#include "directory_parser.h"
#include "feature_detector.h"
#include "car_detector.h"
#include "svm_binder.h"
#include "temp_result_writer.h"
#include <vector>

int main(int argc, char const *argv[])
{
	string posDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/pos_back/";
	string negDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/neg/";
	string testDirName="/home/igor/Work/Thesis/CarData/Freiburg/";

	vector<string> validExtensions;
    validExtensions.push_back("jpg");
    validExtensions.push_back("png");
    validExtensions.push_back("ppm");

	std::vector<string> positiveExsamples;
	std::vector<string> negativeExamples;
	std::vector<string> testExamples;
	DirectoryParser directoryParser;
	positiveExsamples = directoryParser.getFileNames(posDirName, validExtensions);
	negativeExamples = directoryParser.getFileNames(negDirName, validExtensions);
	testExamples = directoryParser.getFileNames(testDirName, validExtensions);

	CarDetector carDetector(
		positiveExsamples,
		negativeExamples,
		testExamples,
		FeatureDetector::SQUARE);
	carDetector.detectCars();

	ResultWriter resultWriter;
	resultWriter.showDetections(carDetector.getDetectedCarRects());

	return 0;
}