#include "directory_parser.h"
#include "feature_detector.h"
#include "car_detector.h"
#include "svm_binder.h"
#include "temp_result_writer.h"
#include <vector>

int main(int argc, char const *argv[])
{
	string posDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/pos/";
	string negDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/neg/";
	string degree_0 = "degree_0/";
	string degree_180 = "degree_180/";
	string degree_plus_45 = "degree_+45/";
	string degree_minus_45 = "degree_-45/";
	string degree_plus_115 = "degree_+115/";
	string degree_minus_115 = "degree_-115/";
	string degree_plus_90 = "degree_+90/";
	string degree_minus_90 = "degree_-90/";
	string negSquare = "neg_square/";
	string negRect = "neg_rect/";
	string testDirName="/home/igor/Work/Thesis/CarData/Freiburg/";

	std::vector<std::string> squarePosDirs;
	std::vector<std::string> rectPosDirs;

	squarePosDirs.push_back(posDirName + degree_0);
	// squarePosDirs.push_back(posDirName + degree_180);

	rectPosDirs.push_back(posDirName + degree_plus_45);
	rectPosDirs.push_back(posDirName + degree_minus_45);
	rectPosDirs.push_back(posDirName + degree_plus_115);
	rectPosDirs.push_back(posDirName + degree_minus_115);
	rectPosDirs.push_back(posDirName + degree_plus_90);
	rectPosDirs.push_back(posDirName + degree_minus_90);

	vector<string> validExtensions;
    validExtensions.push_back("jpg");
    validExtensions.push_back("png");
    validExtensions.push_back("ppm");

	std::vector<string> positiveExsamples;
	std::vector<string> negativeExamples;
	std::vector<string> testExamples;
	DirectoryParser directoryParser;
	std::vector<string> negativeExamplesSquare = directoryParser.getFileNames(negDirName + negSquare, validExtensions);
	std::vector<string> negativeExamplesRect = directoryParser.getFileNames(negDirName + negRect, validExtensions);
	testExamples = directoryParser.getFileNames(testDirName, validExtensions);

	ResultWriter resultWriter;

	for (auto dir: squarePosDirs)
	{
		positiveExsamples = directoryParser.getFileNames(dir, validExtensions);
		CarDetector carDetector(
			positiveExsamples,
			negativeExamplesSquare,
			testExamples,
			FeatureDetector::SQUARE);
		carDetector.detectCars();
		resultWriter.storeDetections(carDetector.getDetectedCarRects());
	}

	for (auto dir: rectPosDirs)
	{
		positiveExsamples = directoryParser.getFileNames(dir, validExtensions);
		CarDetector carDetector(
			positiveExsamples,
			negativeExamplesRect,
			testExamples,
			FeatureDetector::RECTANGLE);
		carDetector.detectCars();
		resultWriter.storeDetections(carDetector.getDetectedCarRects());
	}

	resultWriter.showDetections();

	return 0;
}