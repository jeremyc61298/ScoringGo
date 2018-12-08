// -------------------------------------------------
// go.cpp
// Jeremy Campbell
// Given a final board state, this program will 
// determine who has won the game of Go.
// -------------------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;

struct point {
	int x, y;
	point() : x(0), y(0) {}
	point(int x, int y) : x(x), y(y) {}
};

class ScoreManager {
	vector<vector<char>> gameboard;
	const char WHITE = 'W', BLACK = 'B', UNFILLED = 'U';
	int numWhite, numBlack;

	// Methods
	void readPoints(ifstream& fin, char color, int numColor);

public:
	bool inputBoard(ifstream& fin);

	void printGameboard();
};

bool ScoreManager::inputBoard(ifstream& fin) {
	bool moreCases = false;
	int boardSize;
	gameboard.clear();
	fin >> boardSize;
	
	if (boardSize != 0) {
		gameboard.resize(boardSize, vector<char>(boardSize, UNFILLED));
		fin >> numBlack >> numWhite;
		readPoints(fin, BLACK, numBlack);
		readPoints(fin, WHITE, numWhite);
		moreCases = true;
	}
	return moreCases;
}

void ScoreManager::readPoints(ifstream& fin, char color, int numColor) {
	point curr;
	for (int i = 0; i < numColor; i++) {
		fin >> curr.x >> curr.y;
		gameboard[--curr.x][--curr.y] = color;
	}
}

void ScoreManager::printGameboard() {
	cout << "---------------------------------" << endl;
	for (vector<char>& row : gameboard) {
		for (char spot : row) {
			cout << spot << " ";
		}
		cout << endl;
	}
	cout << "---------------------------------" << endl;
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
		sm.printGameboard();

		moreCases = sm.inputBoard(fin);
		if (moreCases)
			fout << endl;
	}
}