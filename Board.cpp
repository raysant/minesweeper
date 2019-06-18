#include "Board.h"
#include "Tile.h"
#include <FL/Fl_JPEG_Image.H>
#include <iostream>
using namespace std;

void Board::setDificulty(int difficulty, int w, int h, int m) {
	switch (difficulty) {
	case 0: // Beginner
		width = 9;
		height = 9;
		mines = 10;
		break;
	case 1: // Intermediate
		width = 16;
		height = 16;
		mines = 40;
		break;
	case 2: // Expert
		width = 30;
		height = 16;
		mines = 99;
		break;
	case 3: // Custom
		width = w;
		height = h;
		mines = m;
	}
}

void Board::countAdjMines(int x, int y) { //add 1 to all tiles around a mine
	if ((x > 0) && (x < width - 1) && (y > 0) && (y < height - 1)) { // If mine is in the middle of board, add 1 to adjacent tiles
		tiles.at(x - 1).at(y - 1)->incAdjMines();
		tiles.at(x - 1).at(y)->incAdjMines();
		tiles.at(x - 1).at(y + 1)->incAdjMines();
		tiles.at(x).at(y - 1)->incAdjMines();
		tiles.at(x).at(y + 1)->incAdjMines();
		tiles.at(x + 1).at(y - 1)->incAdjMines();
		tiles.at(x + 1).at(y)->incAdjMines();
		tiles.at(x + 1).at(y + 1)->incAdjMines();
	}
	else if ((x == 0) && (y == 0)) { // If mine is in the top left corner
		tiles.at(x).at(y + 1)->incAdjMines();
		tiles.at(x + 1).at(y)->incAdjMines();
		tiles.at(x + 1).at(y + 1)->incAdjMines();
	}
	else if ((x == 0) && (y == height - 1)) { // If mine is in the bottom left corner
		tiles.at(x).at(y - 1)->incAdjMines();
		tiles.at(x + 1).at(y - 1)->incAdjMines();
		tiles.at(x + 1).at(y)->incAdjMines();
	}
	else if ((x == width - 1) && (y == 0)) { // If mine is in the top right corner
		tiles.at(x - 1).at(y)->incAdjMines();
		tiles.at(x - 1).at(y + 1)->incAdjMines();
		tiles.at(x).at(y + 1)->incAdjMines();
	}
	else if ((x == width - 1) && (y == height - 1)) { // If mine is in the bottom right corner
		tiles.at(x - 1).at(y - 1)->incAdjMines();
		tiles.at(x - 1).at(y)->incAdjMines();
		tiles.at(x).at(y - 1)->incAdjMines();
	}
	else if ((x == 0) && !(y == 0) && !(y == height - 1)) { // If mine is in the right edge of board
		tiles.at(x).at(y - 1)->incAdjMines();
		tiles.at(x).at(y + 1)->incAdjMines();
		tiles.at(x + 1).at(y - 1)->incAdjMines();
		tiles.at(x + 1).at(y)->incAdjMines();
		tiles.at(x + 1).at(y + 1)->incAdjMines();
	}
	else if ((x == width - 1) && !(y == 0) && !(y == height - 1)) { // If mine is in the left edge of board
		tiles.at(x).at(y - 1)->incAdjMines();
		tiles.at(x).at(y + 1)->incAdjMines();
		tiles.at(x - 1).at(y - 1)->incAdjMines();
		tiles.at(x - 1).at(y)->incAdjMines();
		tiles.at(x - 1).at(y + 1)->incAdjMines();
	}
	else if ((y == 0) && !(x == 0) && !(x == width -1)) { // If mine is in the top edge of board
		tiles.at(x - 1).at(y)->incAdjMines();
		tiles.at(x - 1).at(y + 1)->incAdjMines();
		tiles.at(x).at(y + 1)->incAdjMines();
		tiles.at(x + 1).at(y)->incAdjMines();
		tiles.at(x + 1).at(y + 1)->incAdjMines();
	}
	else if ((y == height - 1) && !(x == 0) && !(x == width -1)) { // If mines is in the bottom edge of board
		tiles.at(x - 1).at(y - 1)->incAdjMines();
		tiles.at(x - 1).at(y)->incAdjMines();
		tiles.at(x).at(y - 1)->incAdjMines();
		tiles.at(x + 1).at(y - 1)->incAdjMines();
		tiles.at(x + 1).at(y)->incAdjMines();
	}
}

void Board::setAdjMines() {
	/*
	 For tiles with adjacent mines, function assigns # of adjacent mines to tile.
	 Since it is more common to have less tile mines than any other type of tile,
	 this function only runs m(# of mines) times. More efficient than checking 
	 each tile for adjacent mines.
	*/
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (tiles.at(i).at(j)->isTileMine()) {
				countAdjMines(i, j);
			}
		}
	}
}

void Board::setAdjTiles() {
	// Function to set pointers to adjacent tiles around the tile at (i, j)
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (tiles.at(i).at(j)->getAdjMines() == 0) { // Only runs for tiles that have no adjacent mines
				if ((i > 0) && (i < width - 1) && (j > 0) && (j < height - 1)) { // If tile at (i, j) is in the middle of board
					tiles.at(i).at(j)->top = tiles.at(i).at(j - 1);
					tiles.at(i).at(j)->topLeft = tiles.at(i - 1).at(j - 1);
					tiles.at(i).at(j)->topRight = tiles.at(i + 1).at(j - 1);
					tiles.at(i).at(j)->bottom = tiles.at(i).at(j + 1);
					tiles.at(i).at(j)->bottomLeft = tiles.at(i - 1).at(j + 1);
					tiles.at(i).at(j)->bottomRight = tiles.at(i + 1).at(j + 1);
					tiles.at(i).at(j)->left = tiles.at(i - 1).at(j);
					tiles.at(i).at(j)->right = tiles.at(i + 1).at(j);
				}
				else if ((i == 0) && (j == 0)) { // If tile is in the top left corner
					tiles.at(i).at(j)->bottom = tiles.at(i).at(j + 1);
					tiles.at(i).at(j)->bottomRight = tiles.at(i + 1).at(j + 1);
					tiles.at(i).at(j)->right = tiles.at(i + 1).at(j);
					tiles.at(i).at(j)->top = NULL;
					tiles.at(i).at(j)->topLeft = NULL;
					tiles.at(i).at(j)->topRight = NULL;
					tiles.at(i).at(j)->bottomLeft = NULL;
					tiles.at(i).at(j)->left = NULL;
				}
				else if ((i == 0) && (j == height - 1)) { // If tile is in the bottom left corner
					tiles.at(i).at(j)->top = tiles.at(i).at(j - 1);
					tiles.at(i).at(j)->topRight = tiles.at(i + 1).at(j - 1);
					tiles.at(i).at(j)->right = tiles.at(i + 1).at(j);
					tiles.at(i).at(j)->topLeft = NULL;
					tiles.at(i).at(j)->bottom = NULL;
					tiles.at(i).at(j)->bottomLeft = NULL;
					tiles.at(i).at(j)->bottomRight = NULL;
					tiles.at(i).at(j)->left = NULL;
				}
				else if ((i == width - 1) && (j == 0)) { // If tile is in the top right corner
					tiles.at(i).at(j)->bottom = tiles.at(i).at(j + 1);
					tiles.at(i).at(j)->bottomLeft = tiles.at(i - 1).at(j + 1);
					tiles.at(i).at(j)->left = tiles.at(i - 1).at(j);
					tiles.at(i).at(j)->top = NULL;
					tiles.at(i).at(j)->topLeft = NULL;
					tiles.at(i).at(j)->topRight = NULL;
					tiles.at(i).at(j)->bottomRight = NULL;
					tiles.at(i).at(j)->right = NULL;
				}
				else if ((i == width - 1) && (j == height - 1)) { // If tile is in the bottom right corner
					tiles.at(i).at(j)->top = tiles.at(i).at(j - 1);
					tiles.at(i).at(j)->topLeft = tiles.at(i - 1).at(j - 1);
					tiles.at(i).at(j)->left = tiles.at(i - 1).at(j);
					tiles.at(i).at(j)->topRight = NULL;
					tiles.at(i).at(j)->bottom = NULL;
					tiles.at(i).at(j)->bottomLeft = NULL;
					tiles.at(i).at(j)->bottomRight = NULL;
					tiles.at(i).at(j)->right = NULL;
				}
				else if ((i == 0) && !(j == 0) && !(j == height - 1)) { // If tile is in the left edge
					tiles.at(i).at(j)->top = tiles.at(i).at(j - 1);
					tiles.at(i).at(j)->topRight = tiles.at(i + 1).at(j - 1);
					tiles.at(i).at(j)->bottom = tiles.at(i).at(j + 1);
					tiles.at(i).at(j)->bottomRight = tiles.at(i + 1).at(j + 1);
					tiles.at(i).at(j)->right = tiles.at(i + 1).at(j);
					tiles.at(i).at(j)->topLeft = NULL;
					tiles.at(i).at(j)->bottomLeft = NULL;
					tiles.at(i).at(j)->left = NULL;
				}
				else if ((i == width - 1) && !(j == 0) && !(j == height - 1)) { // If tile is in the right edge
					tiles.at(i).at(j)->top = tiles.at(i).at(j - 1);
					tiles.at(i).at(j)->topLeft = tiles.at(i - 1).at(j - 1);
					tiles.at(i).at(j)->bottom = tiles.at(i).at(j + 1);
					tiles.at(i).at(j)->bottomLeft = tiles.at(i - 1).at(j + 1);
					tiles.at(i).at(j)->left = tiles.at(i - 1).at(j);
					tiles.at(i).at(j)->topRight = NULL;
					tiles.at(i).at(j)->bottomRight = NULL;
					tiles.at(i).at(j)->right = NULL;
				}
				else if ((j == 0) && !(i == 0) && !(i == width - 1)) { // If tile is in the top edge
					tiles.at(i).at(j)->bottom = tiles.at(i).at(j + 1);
					tiles.at(i).at(j)->bottomLeft = tiles.at(i - 1).at(j + 1);
					tiles.at(i).at(j)->bottomRight = tiles.at(i + 1).at(j + 1);
					tiles.at(i).at(j)->left = tiles.at(i - 1).at(j);
					tiles.at(i).at(j)->right = tiles.at(i + 1).at(j);
					tiles.at(i).at(j)->top = NULL;
					tiles.at(i).at(j)->topLeft = NULL;
					tiles.at(i).at(j)->topRight = NULL;
				}
				else if ((j == height - 1) && !(i == 0) && !(i == width - 1)) { // If tile is in the bottom edge
					tiles.at(i).at(j)->top = tiles.at(i).at(j - 1);
					tiles.at(i).at(j)->topLeft = tiles.at(i - 1).at(j - 1);
					tiles.at(i).at(j)->topRight = tiles.at(i + 1).at(j - 1);
					tiles.at(i).at(j)->left = tiles.at(i - 1).at(j);
					tiles.at(i).at(j)->right = tiles.at(i + 1).at(j);
					tiles.at(i).at(j)->bottom = NULL;
					tiles.at(i).at(j)->bottomLeft = NULL;
					tiles.at(i).at(j)->bottomRight = NULL;
				}
			}
			else { // Assign default pointers to tiles that have adjacent mines
				tiles.at(i).at(j)->top = NULL;
				tiles.at(i).at(j)->topLeft = NULL;
				tiles.at(i).at(j)->topRight = NULL;
				tiles.at(i).at(j)->bottom = NULL;
				tiles.at(i).at(j)->bottomLeft = NULL;
				tiles.at(i).at(j)->bottomRight = NULL;
				tiles.at(i).at(j)->left = NULL;
				tiles.at(i).at(j)->right = NULL;
			}
		}
	}
}

bool Board::checkIfWon() {
	// Check if the game is won
	if ((width * height - tilesClicked) == mines) {
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				if (tiles.at(i).at(j)->isTileMine()) { // Flag mines when won
					tiles.at(i).at(j)->label(0);
					tiles.at(i).at(j)->deimage(images.at(2));
					tiles.at(i).at(j)->deactivate();
				}
			}
		}
		timeStart = false;
		window->color(FL_DARK_GREEN); // Make window green when game is wom
		window->redraw();
		return true;
	}
	return false;
}

void Board::newGame(int s) {
	tiles.clear();
	tiles.resize(width, vector<Tile*>(height));
	int minesLeft = mines;
	int row = 0;
	int col = 0;
	gameEnd = false;
	tilesClicked = 0;
	flagsLeft = mines;

	seed = s;
	srand(seed);

	// Place tiles on the window
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			tiles.at(i).at(j) = new Tile(i * 18 + 35, j * 18 + 120, 18, 18);
		}
	}

	// Added to prevent unnecessary loops
	if (minesLeft > (width * height)) {
		minesLeft = width * height;
	}

	// Randomly assign mine tiles in new game
	while (minesLeft > 0) {
		row = rand() % width;
		col = rand() % height;
		if (!(tiles.at(row).at(col)->isTileMine())) {
			tiles.at(row).at(col)->setMine(true);
			if (debugMines) {
				tiles.at(row).at(col)->labelcolor(FL_DARK_RED);
				tiles.at(row).at(col)->copy_label("M");
			}
			--minesLeft;
		}
	}

	Board::setAdjMines();
	Board::setAdjTiles();
}

Board::Board() : width(9), height(9), mines(10), debugMines(false), gameEnd(false), timeStart(false), flagsLeft(0), seed(0) {
	Board::newGame();
}