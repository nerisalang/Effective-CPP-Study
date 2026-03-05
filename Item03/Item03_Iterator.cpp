#include<iostream>
#include<vector>
using namespace std;

int main() {
	vector<int> vec = {1,2,3};

	const vector<int>::iterator iter = vec.begin();
	*iter = 10;
	++iter;

	vector<int>::const_iterator cIter = vec.begin();
	*cIter = 20;
	++cIter;

	return 0;
}
