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

#include "directory_parser.h"
#include "feature_detector.h"
#include "car_detector.h"
#include "svm_binder.h"
#include "lasers_parser.h"
#include "temp_result_writer.h"
#include "sequence_generator.h"
#include "utils.h"
#include <vector>

int main(int argc, char const *argv[])
{
	std::string folder, dateOfData;
	Utils::analyzeCmdParams(argc, argv, folder, dateOfData);
	string posDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/pos/";
	string negDirName="/home/igor/Work/Thesis/MiscCode/HOGclassifier/neg/";
	string degree_0 = "degree_0";
	string degree_0_180 = "degree_0_180";
	string degree_180 = "degree_180";
	string degree_plus_45 = "degree_+45";
	string degree_minus_45 = "degree_-45";
	string degree_plus_115 = "degree_+115";
	string degree_minus_115 = "degree_-115";
	string degree_plus_90 = "degree_+90";
	string degree_minus_90 = "degree_-90";
	string negSquare = "neg_square/";
	string negRect = "neg_rect/";
	string resultDir="/home/igor/Work/Thesis/MiscCode/HOGclassifier/Result/";
	string logfile="/home/igor/Work/Thesis/MiscCode/HOGclassifier/Result/car_pos_" + dateOfData + ".dat";
	// string depthDirName="/home/igor/Work/Thesis/CarData/CarSeasonsNewAll/Rectified/Depth/";
	// string allLeftImagesDir = "/home/igor/Work/Thesis/CarData/CarSeasonsNewAll/Rectified/Left/";
	// string allRightImagesDir = "/home/igor/Work/Thesis/CarData/CarSeasonsNewAll/Rectified/Right/";
	string allLeftImagesDir = folder + "/img/Left/";
	string allRightImagesDir = folder + "/img/Right/";
	string laserLogFileName = "/home/igor/Work/Thesis/MiscCode/PythonScripts/laser_points_" + dateOfData + ".txt";

	std::vector<std::string> squarePosDirs;
	std::vector<std::string> rectPosDirs;

	squarePosDirs.push_back(posDirName + degree_0_180 +"/");
	// squarePosDirs.push_back(posDirName + degree_180 + "/");

	// rectPosDirs.push_back(posDirName + degree_plus_45 + "/");
	// rectPosDirs.push_back(posDirName + degree_minus_45 + "/");
	// rectPosDirs.push_back(posDirName + degree_plus_115 + "/");
	// rectPosDirs.push_back(posDirName + degree_minus_115 + "/");
	// rectPosDirs.push_back(posDirName + degree_plus_90 + "/");
	// rectPosDirs.push_back(posDirName + degree_minus_90 + "/");

	vector<string> validExtensions;
    validExtensions.push_back("jpg");
    validExtensions.push_back("png");
    validExtensions.push_back("ppm");

	std::vector<string> positiveExsamples;
	std::vector<string> negativeExamples;
	DirectoryParser directoryParser;
	std::vector<string> negativeExamplesSquare = directoryParser.getFileNames(negDirName + negSquare, validExtensions);
	std::vector<string> negativeExamplesRect = directoryParser.getFileNames(negDirName + negRect, validExtensions);
	std::vector<string> testExamples = directoryParser.getFileNames(allLeftImagesDir, validExtensions);
	std::unordered_map<string, string> leftToRightNamesMapping = directoryParser.getLeftRightImagePairs(
		allLeftImagesDir, allRightImagesDir, validExtensions);

	int seed = 10;
	int numberOfTestSamples = 2000;
	int gap = 10;
	int minIndex = 50;
	SequenceGenerator sequenceGenerator(seed, numberOfTestSamples, minIndex, testExamples.size(), gap);
	sequenceGenerator.generateSequences(SequenceGenerator::SEQUENTIAL);
	testExamples = sequenceGenerator.getTestExamples(testExamples);

	ResultWriter resultWriter(testExamples, logfile);
	LaserParser laserParser(laserLogFileName);

	for (auto dir: squarePosDirs)
	{
		positiveExsamples = directoryParser.getFileNames(dir, validExtensions);
		CarDetector carDetector(
			positiveExsamples,
			negativeExamplesSquare,
			testExamples,
			FeatureDetector::SQUARE);
		carDetector.detectCars(dir + "model.dat", CarDetector::LEAVE_OLD_MODEL);
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
		carDetector.detectCars(dir + "model.dat", CarDetector::LEAVE_OLD_MODEL);
		resultWriter.storeDetections(carDetector.getDetectedCarRects());
	}

	////a version that uses depth from camera
	// resultWriter.showDetections(leftToRightNamesMapping, resultDir);

	//a version that uses depth from lasers
	resultWriter.showDetectionsLaser(leftToRightNamesMapping, resultDir, laserParser);

	return 0;
}