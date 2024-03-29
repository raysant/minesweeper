/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ray Santana
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
	void setDificulty(int difficulty, int w = 8, int h = 2, int m = 0);
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
