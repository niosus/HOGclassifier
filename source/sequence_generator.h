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
	SequenceGenerator(int seed, int testSize, int minIndex, int maxIndex, int gap);
	SequenceGenerator():_seed(-1), _testSize(-1), _maxIndex(-1), _gap(1) {}
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
	int _gap;
	vector<int> _testSeq;
	OrderType _type;

	void generateTest(OrderType type);
};

#endif