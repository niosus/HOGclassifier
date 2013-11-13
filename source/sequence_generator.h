#ifndef SEQUENCE_GEN_H
#define SEQUENCE_GEN_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "logger.h"
using namespace std;


class SequenceGenerator
{
public:
	enum OrderType {RANDOM, SEQUENTIAL};
	SequenceGenerator(int seed, int testSize, int minIndex, int maxIndex);
	SequenceGenerator():_seed(-1), _testSize(-1), _maxIndex(-1) {}
	bool generateSequences(OrderType type);
	bool checkContent();
	vector<int> getTrainSeq();
	vector<int> getTestSeq();
	vector<string> getTestExamples(const std::vector<string> &allExamples);

private:
	int _seed;
	int _testSize;
	int _maxIndex;
	int _minIndex;
	vector<int> _testSeq;
	OrderType _type;

	void generateTest(OrderType type);
};

#endif