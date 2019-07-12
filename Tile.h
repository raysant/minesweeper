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
#include <FL/Fl_Button.H>
#include <FL/Fl_JPEG_Image.H>

class Tile : public Fl_Button {
public:
	Tile(int x, int y, int w, int h, const char *label = 0);
	int handle(int event);
	void incAdjMines() { ++adjacentMines; }
	void setFlag(bool flagged) { isFlag = flagged; }
	void setMine(bool mine) { isMine = mine; }
	void setUncovered(bool c) { uncovered = c; }
	void setAdjacentMines(int mines) { adjacentMines = mines; }
	void setCascade(bool c) { cascade = c; }
	int getAdjMines() { return adjacentMines; }
	bool isCascade() { return cascade; }
	bool isTileFlag() { return isFlag; }
	bool isTileMine() { return isMine; }
	bool isTileQuestion() { return isQuestion; }
	bool isUncovered() { return uncovered; }
	Tile* top;
	Tile* topLeft;
	Tile* topRight;
	Tile* bottom;
	Tile* bottomLeft;
	Tile* bottomRight;
	Tile* left;
	Tile* right;
private:
	bool isFlag;
	bool isMine;
	bool isQuestion;
	bool uncovered;
	bool cascade;
	int adjacentMines;
};
