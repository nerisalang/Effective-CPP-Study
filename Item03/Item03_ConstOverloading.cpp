#include<iostream>
#include<string>
using namespace std;

class TextBlock {
public:
	TextBlock(string text) : text(text) {}

	char& operator[](size_t position) {
		cout << "(일반 호출)";
		return text[position];
	}

	const char& operator[](size_t position) const {
		cout << "(상수호출)";
		return text[position];
	}

private:
	string text;
};

int main() {
	TextBlock tb("Hello");
	const TextBlock ctb("World");

	cout << tb[0] <<endl;
	cout << ctb[0] <<endl;

	tb[0] = 'X';
	// ctb[0] = 'X';

	return 0;
}
			
