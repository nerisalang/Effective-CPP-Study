#include<iostream>
using namespace std;

class GamePlayer {
public:
	enum { NumTurns = 5 };

	int scores[NumTurns];
};

int main() {
	GamePlayer player;

	cout<< "Turns: " << GamePlayer::NumTurns << endl;

	return 0;
}
