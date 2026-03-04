#include<iostream>
#include<string>

class GamePlayer {
public:
	static const int NumTurns = 5;
	int scores[NumTurns];
};

int main() {
	const char* const authorName = "Minho Kim";

	const std::string authorStr("Minho Kim");

	GamePlayer player;

	std::cout << "Author: " <<authorName << std::endl;
	std::cout << "Turns: " <<GamePlayer::NumTurns <<std::endl;

	return 0;
}
