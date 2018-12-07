// -------------------------------------------------
// go.cpp
// Jeremy Campbell
// Given a final board state, this program will 
// determine who has won the game.
// -------------------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;

class ScoreManager {
	vector<vector<char>> gameboard;
	const char WHITE = 'W', BLACK = 'B', UNFILLED = 'U';
	int numWhite, numBlack;
public:
	bool inputBoard(ifstream& fin);
};

bool ScoreManager::inputBoard(ifstream& fin) {
	bool moreCases = false;
	int boardSize;
	fin >> boardSize;

	gameboard.resize(boardSize, vector<char>(boardSize, UNFILLED));
	
	if (boardSize != 0) {
		fin >> numBlack >> numWhite;
		for (int i = 0; i < numBlack; i++) {

		}

		moreCases = true;
	}
	
	return moreCases;
}

int main() {
	ifstream fin("go.in");
	if (!fin) {
		cerr << "Cannot find the input file." << endl;
		return -1;
	}

	ofstream fout("go.out");
	ScoreManager sm;
	bool moreCases = sm.inputBoard(fin);

	while (moreCases) {

		moreCases = sm.inputBoard(fin);
		if (moreCases)
			fout << endl;
	}
}