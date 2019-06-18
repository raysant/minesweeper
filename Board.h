#pragma once
#include <vector>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <ctime>
#include "Tile.h"
using namespace std;

enum difficulty { BEGINNER, INTERMEDIATE, EXPERT, CUSTOM };

class Board {
public:
	Board();
	void setDificulty(int difficulty, int w = 2, int h = 2, int m = 0);
	void countAdjMines(int x, int y);
	void incTilesClicked() { ++tilesClicked; }
	bool checkIfWon();
	bool checkDebugMines() { return debugMines; }
	bool checkGameEnd() { return gameEnd; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getMines() { return mines; }
	int getSeedNum() { return seed; }
	void setAdjTiles();
	void setAdjMines();
	void newGame(int s = time(0));
	void setGameEnd(bool s) { gameEnd = s; }
	void setDebugMines(bool d) { debugMines = d; }
	vector<Fl_JPEG_Image*> images;
	vector<vector<Tile*>> tiles;
	Fl_Window* window;
	Fl_Button* restart;
	Fl_Menu_Bar* menuBar;
	Fl_Box* flagCounts;
	Fl_Box* timer;
	int flagsLeft;
	bool timeStart;
	int t;
private:
	int width;
	int height;
	int mines;
	int tilesClicked;
	bool debugMines;
	bool gameEnd;
	int seed;
};
