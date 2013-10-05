#ifndef SVM_BINDER_H
#define SVM_BINDER_H

#include "svm.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include "patch.h"
using namespace std;

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

class SvmBinder
{
private:
	struct svm_model *_model;
	struct svm_parameter _param;
	struct svm_problem _probTrain;
	struct svm_problem _probTest;

	vector <double > _hyperPlane;
	vector<int> _newLabels;
	double _bias;
	int _versor;
	string _modelPath;

	void setProblemTest(vector<vector<float> >* hogs);
	void setProblemTrain(
		vector<vector<float> >* hogsPos, 
		vector<vector<float> >* hogsNeg);
public:
	SvmBinder(); //maybe add some parameters for non default parameters of svn
	~SvmBinder();

	string& modelPath();
	bool hasModelPath();

	bool loadModel();
	void saveModel();

	void createHyperPlane();
	void train(
		vector<vector<float> >* hogsPos, 
		vector<vector<float> >* hogsNeg);
	vector<int>* test(vector<vector<float> >* hogs);
	vector<int>* testWithHyperPlane(vector<vector<float> >* hogs);	
};

#endif