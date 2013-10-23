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
  int tenPercentChunk = filenames.size()/100;
	for (int i=0; i< filenames.size(); ++i)
	{
    if (i%tenPercentChunk==0)
      Logger::instance()->logInfo("done", i/tenPercentChunk);

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

void FeatureDetector::setTestHogFromHyperplane(std::vector<float>* hyperplane)
{
  _hogTestCpu.setSVMDetector(*hyperplane);
	_hogTestGpu.setSVMDetector(*hyperplane);
}

std::map<std::string, std::vector<cv::Rect> > FeatureDetector::detectMultiScaleGpu(std::vector<std::string>& imageNames)
{
  std::map<std::string, std::vector<cv::Rect> > allFoundRects;
	std::vector<cv::Rect> found;
  int groupThreshold = 2;
  cv::Size padding(cv::Size(0, 0));
  cv::Size winStride(cv::Size(8, 8));
  double hitThreshold = 0.0; // tolerance
  cv::gpu::GpuMat testImage;
	for (auto imageName: imageNames)
	{
		cv::Mat testImage_cpu = cv::imread(imageName.c_str(), CV_LOAD_IMAGE_COLOR);
    cv::Mat testImage_cpu_rgba;
    cv::cvtColor(testImage_cpu, testImage_cpu_rgba, CV_BGR2BGRA);
    testImage.upload(testImage_cpu_rgba);
		_hogTestGpu.detectMultiScale(testImage, found, hitThreshold, winStride, padding, 1.05, groupThreshold);
    for (cv::Rect rect: found)
    {
      allFoundRects[imageName].push_back(rect);
    }
    Logger::instance()->logInfo("computed hogs for", imageName);
	}
  return allFoundRects;
}

std::map<std::string, std::vector<cv::Rect> > FeatureDetector::detectMultiScaleCpu(std::vector<std::string>& imageNames)
{
  std::map<std::string, std::vector<cv::Rect> > allFoundRects;
  std::vector<cv::Rect> found;
  int groupThreshold = 2;
  cv::Size padding(cv::Size(32, 32));
  cv::Size winStride(cv::Size(8, 8));
  double hitThreshold = 0.0; // tolerance
  for (auto imageName: imageNames)
  {
    cv::Mat testImage = cv::imread(imageName.c_str(), CV_LOAD_IMAGE_COLOR);
    _hogTestCpu.detectMultiScale(testImage, found, hitThreshold, winStride, padding, 1.05, groupThreshold);
    for (cv::Rect rect: found)
    {
      allFoundRects[imageName].push_back(rect);
    }
    Logger::instance()->logInfo("computed hogs for", imageName);
  }
  return allFoundRects;
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
