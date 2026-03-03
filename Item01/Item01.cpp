#include<iostream>
#include<vector>
using namespace std;

extern "C" void c_style_func(int a){}

class Player {
public:
	void attack(int damage){}
};

int main()
{
	c_style_func(10);
	Player p;
	p.attack(50);
	vector<int> v;
	v.push_back(1);
	return 0;
}
