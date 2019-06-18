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
