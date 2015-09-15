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

int main(int argc, char const *argv[]) {
    // std::string folder, dateOfData;
    // Utils::analyzeCmdParams(argc, argv, folder, dateOfData);
    string posDirName = "/home/igor/Data/pcl_icp/hogs_train/pos/";
    string negDirName = "/home/igor/Data/pcl_icp/hogs_train/neg/";
    string testDirName = "/home/igor/Data/pcl_icp/hogs_train/test/";
    string resultDirName = "/home/igor/Data/pcl_icp/hogs_train/res/";
    std::vector<std::string> squarePosDirs;

    squarePosDirs.push_back(posDirName);

    vector<string> validExtensions;
    validExtensions.push_back("jpg");
    validExtensions.push_back("png");
    validExtensions.push_back("ppm");

    std::vector<string> positiveExsamples;
    std::vector<string> negativeExamples;
    DirectoryParser directoryParser;
    std::vector<string> negativeExamplesSquare = directoryParser.getFileNames(negDirName, validExtensions);
    std::vector<string> testExamples = directoryParser.getFileNames(testDirName, validExtensions);

    int seed = 10;
    int numberOfTestSamples = 10;
    int gap = 0;
    int minIndex = 0;
    SequenceGenerator sequenceGenerator(seed, numberOfTestSamples, minIndex, testExamples.size(), gap);
    sequenceGenerator.generateSequences(SequenceGenerator::SEQUENTIAL);
    testExamples = sequenceGenerator.getTestExamples(testExamples);

    string logfile = "/home/igor/Data/pcl_icp/hogs_train/res/result.dat";
    ResultWriter resultWriter(testExamples, logfile);

    for (auto dir : squarePosDirs) {
        positiveExsamples = directoryParser.getFileNames(dir, validExtensions);
        CarDetector carDetector(
            positiveExsamples,
            negativeExamplesSquare,
            testExamples,
            FeatureDetector::SQUARE);
        carDetector.detectCars(dir + "model.dat", CarDetector::LEAVE_OLD_MODEL);
        resultWriter.storeDetections(carDetector.getDetectedCarRects());
    }

    // for (auto dir : rectPosDirs) {
    //     positiveExsamples = directoryParser.getFileNames(dir, validExtensions);
    //     CarDetector carDetector(
    //         positiveExsamples,
    //         negativeExamplesRect,
    //         testExamples,
    //         FeatureDetector::RECTANGLE);
    //     carDetector.detectCars(dir + "model.dat", CarDetector::LEAVE_OLD_MODEL);
    //     resultWriter.storeDetections(carDetector.getDetectedCarRects());
    // }

    // a version that uses depth from camera
    resultWriter.showDetections(resultDirName);

    // a version that uses depth from lasers
    // resultWriter.showDetectionsLaser(leftToRightNamesMapping, resultDir, laserParser);
    return 0;
}
