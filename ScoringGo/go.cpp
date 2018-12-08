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
#include <stack>
#include <unordered_set>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::stack;
using std::unordered_set;
using std::to_string;

struct point {
	int x, y;
	point() : x(0), y(0) {}
	point(int x, int y) : x(x), y(y) {}
	string str() {
		return to_string(x) + " " + to_string(y);
	}
};

class ScoreManager {
	vector<vector<char>> gameboard;
	unordered_set<string> markedPoints;
	const char WHITE = 'W', BLACK = 'B', UNFILLED = 'U', WALL = '#', null = 'N';
	static const int numDirections = 4;
	int numWhite, numBlack, whiteScore, blackScore;

	// Methods
	void readPoints(ifstream& fin, char color, int numColor);
	void initializeBoard(int boardSize);
	void determineOwner(point p);
	int testNeighbors(point neighbor[], stack<point>& pointStack, 
		char& firstColor, bool& firstColorFound, bool& owned);
	char getAt(point p);
	void addToScore(char color, int score);
	bool marked(point p);
	void mark(point p);

public:
	bool inputBoard(ifstream& fin);
	void scoreGame();
	void printScore(ofstream& fout);
	void printGameboard(ofstream& fout);
};

void ScoreManager::initializeBoard(int boardSize) {
	vector<char> endrow(boardSize + 2, WALL);
	vector<char> row(boardSize + 2);
	row[0] = WALL;
	for (int i = 1; i <= boardSize; i++) {
		row[i] = UNFILLED;
	}
	row[boardSize + 1] = WALL;

	// Push back the rows
	gameboard.push_back(endrow);
	for (int i = 1; i <= boardSize; i++) {
		gameboard.push_back(row);
	}
	gameboard.push_back(endrow);
}

bool ScoreManager::inputBoard(ifstream& fin) {
	bool moreCases = false;
	int boardSize;
	gameboard.clear();
	markedPoints.clear();
	fin >> boardSize;
	
	if (boardSize != 0) {
		initializeBoard(boardSize);
		fin >> numBlack >> numWhite;
		readPoints(fin, BLACK, numBlack);
		readPoints(fin, WHITE, numWhite);
		blackScore = whiteScore = 0;
		moreCases = true;
	}
	return moreCases;
}

void ScoreManager::readPoints(ifstream& fin, char color, int numColor) {
	point curr;
	for (int i = 0; i < numColor; i++) {
		fin >> curr.x >> curr.y;
		gameboard[curr.x][curr.y] = color;
	}
}

void ScoreManager::scoreGame() {
	// Search for an unfilled position
	size_t boardSize = gameboard.size();
	for (int row = 1; row < boardSize; row++) {
		for (int col = 1; col < boardSize; col++) {
			point p(row, col);
			if (gameboard[row][col] == UNFILLED && !marked(p)) {
				determineOwner(p);
			}
		}
	}
}

void ScoreManager::determineOwner(point p) {
	// First find an adjacent player token
	int totalPointsInRegion = 1;
	char firstColor = null;
	bool owned = true, firstColorFound = false;
	stack<point> pointStack;
	pointStack.push(p);
	mark(p);

	while (!pointStack.empty()) {
		// Discover and mark the entirety of this unfilled section
		point curr = pointStack.top(); pointStack.pop();
		point neighbors[numDirections]{
			point(curr.x - 1, curr.y),
			point(curr.x, curr.y + 1),
			point(curr.x + 1, curr.y),
			point(curr.x, curr.y - 1)
		};

		totalPointsInRegion += testNeighbors(neighbors, pointStack, 
			firstColor, firstColorFound, owned);
	}
	if (owned) {
		addToScore(firstColor, totalPointsInRegion);
	}
}

int ScoreManager::testNeighbors(point neighbors[], stack<point>& pointStack, 
								char& firstColor, bool& firstColorFound, bool& owned) {
	int totalPointsInRegion = 0;
	for (int i = 0; i < numDirections; i++) {
		char color = getAt(neighbors[i]);
		if (owned && (color == WHITE || color == BLACK)) {
			if (!firstColorFound) {
				firstColor = color;
				firstColorFound = true;
			}
			owned = (color == firstColor);
		}
		else if (color == UNFILLED && !marked(neighbors[i])) {
			mark(neighbors[i]);
			pointStack.push(neighbors[i]);
			totalPointsInRegion++;
		}
	}
	return totalPointsInRegion;
}

// Returns the character stored at point p in the gameboard
char ScoreManager::getAt(point p) {
	return gameboard[p.x][p.y];
}

void ScoreManager::addToScore(char color, int score) {
	if (color == WHITE)
		whiteScore += score;
	else
		blackScore += score;
}

bool ScoreManager::marked(point p) {
	return markedPoints.find(p.str()) != markedPoints.end();
}

void ScoreManager::mark(point p) {
	markedPoints.insert(p.str());
}

void ScoreManager::printScore(ofstream& fout) {
	if (whiteScore > blackScore) {
		fout << "white wins by " << whiteScore - blackScore;
	}
	else if (blackScore > whiteScore) {
		fout << "black wins by " << blackScore - whiteScore;
	}
	else {
		fout << "draw";
	}
}

static int numGame = 1;
void ScoreManager::printGameboard(ofstream& fout) {
	fout << numGame++ << endl;
	fout << "---------------------------------" << endl;
	for (vector<char>& row : gameboard) {
		for (char spot : row) {
			fout << spot << " ";
		}
		fout << endl;
	}
	fout << "---------------------------------" << endl;
}

int main() {
	ifstream fin("go.in");
	if (!fin) {
		cerr << "Cannot find the input file." << endl;
		return -1;
	}

	ofstream fout("go.out");
	//ofstream map("go.map");
	ScoreManager sm;
	bool moreCases = sm.inputBoard(fin);

	while (moreCases) {
		//sm.printGameboard(map);
		sm.scoreGame();
		sm.printScore(fout);
		moreCases = sm.inputBoard(fin);
		if (moreCases)
			fout << endl;
	}
}