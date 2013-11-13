#include "sequence_generator.h"
using namespace std;

SequenceGenerator::SequenceGenerator(int seed, int testSize, int minIndex, int maxIndex)
{
	_seed = seed;
	_testSize = testSize;
	_maxIndex = maxIndex;
	_minIndex = minIndex;
}

bool SequenceGenerator::generateSequences(OrderType type)
{

	if (checkContent())
	{
		generateTest(type);
		return true;
	}
	return false;
}

bool SequenceGenerator::checkContent()
{
	if (_seed==-1 
		|| _testSize == -1
		|| _maxIndex == -1)
	{
		Logger::instance()->logError("SequenceGenerator", Logger::NO_SEQUENCE);
		return false;
	}
	return true;
}

void SequenceGenerator::generateTest(OrderType type)
{
	int item;
	switch (type)
	{
		case RANDOM:
			srand(_seed);
			for(int i=0; i<_testSize; i++)
			{
				item = _minIndex + rand() % (_maxIndex - _minIndex);
				if (std::find(_testSeq.begin(), _testSeq.end(), item)==_testSeq.end())
				{
					_testSeq.push_back(item);
				}
				else i--;
			}
			std::sort(_testSeq.begin(), _testSeq.end());
			cout<<"Test Sequence \n";
			for(size_t i=0; i<_testSeq.size(); ++i)
				cout<<_testSeq[i]<<" ";
			cout<<endl;
			break;
		case SEQUENTIAL:
			for(int i=0; i<_testSize; i++)
			{
				item = _minIndex + i;
				if (item < _maxIndex)
				{
					_testSeq.push_back(item);
				}
				else break;
			}
			std::sort(_testSeq.begin(), _testSeq.end());
			cout<<"Test Sequence \n";
			for(size_t i=0; i<_testSeq.size(); ++i)
				cout<<_testSeq[i]<<" ";
			cout<<endl;
			break;
	}
}

vector<int> SequenceGenerator::getTestSeq()
{
	return _testSeq;
}

vector<string> SequenceGenerator::getTestExamples(const std::vector<string> &allExamples)
{
	std::vector<string> res;
	res.reserve(_testSeq.size());
	for (auto randIndex: _testSeq)
	{
		res.push_back(allExamples[randIndex]);
	}
	return res;
}