//#include <Windows.h> //uncomment to run on Windows
#include <FL/Fl_Window.H>
#include <FL/Fl.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Box.H>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "Board.h"
#include "Tile.h"
using namespace std;

bool displayFunctionCalls = false;
bool showMineLocations = false;
bool displayVariables = false;

void timeCb(void* g) {
	// Function to show elapsed time
	Board* game = static_cast<Board*>(g);
	if (game->timeStart) {
		++game->t;
		string tVal = to_string(game->t);
		game->timer->copy_label(tVal.c_str());
		Fl::repeat_timeout(1.0, timeCb, game);
	}
}

void getImages(vector<Fl_JPEG_Image*>& imgVector) {
	// Loads images for the game
	int imgAmount = 6;
	imgVector.resize(imgAmount);

	// Game images
	imgVector.at(0) = new Fl_JPEG_Image("images/mine.jpg");
	imgVector.at(1) = new Fl_JPEG_Image("images/mineClicked.jpg");
	imgVector.at(2) = new Fl_JPEG_Image("images/flag.jpg");
	imgVector.at(3) = new Fl_JPEG_Image("images/question.jpg");
	imgVector.at(4) = new Fl_JPEG_Image("images/mineWrong.jpg");
	// Image of moi
	imgVector.at(5) = new Fl_JPEG_Image("dev/ray.jpg");
}

void setTileImages(Board& g) {
	// If tile is a mine, function assigns the mine image
	// that will be displayed when the game is lost
	for (int i = 0; i < g.getWidth(); ++i) {
		for (int j = 0; j < g.getHeight(); ++j) {
			if (g.tiles.at(i).at(j)->isTileMine()) {
				g.tiles.at(i).at(j)->deimage(*g.images.at(0));
			}
		}
	}
}

void tileDefaultCallback(Fl_Widget* widget, void* g) {
	// Default callback for tiles that have adjacent mines
	Tile* tile = dynamic_cast<Tile*>(widget);
	Board* game = static_cast<Board*>(g);

	if (!game->timeStart) { // Starts time
		game->timeStart = true;
		Fl::add_timeout(1.0, timeCb, game);
	}

	if (tile->isTileFlag() && !tile->isCascade()) { // Places flag image on tile that is right clicked
		tile->image(game->images.at(2));
		--game->flagsLeft;
		string val = to_string(game->flagsLeft);
		game->flagCounts->copy_label(val.c_str());
		if (displayVariables) {
			cout << "Flags Left: " << game->flagsLeft << endl;
		}
		return;
	}
	else if (tile->isTileQuestion() && !tile->isCascade()) { // Places question image
		tile->image(game->images.at(3));
		++game->flagsLeft;
		string val = to_string(game->flagsLeft);
		game->flagCounts->copy_label(val.c_str());
		if (displayVariables) {
			cout << "Flags Left: " << game->flagsLeft << endl;
		}
		return;
	}

	tile->setUncovered(true);
	game->incTilesClicked(); // Add 1 to number of tiles uncovered
	game->setGameEnd(game->checkIfWon()); // Check if the game ended/won
	if (displayFunctionCalls) {
		if (game->checkGameEnd()) {
			cout << "You Won!!!" << endl << endl;
		}
	}
}

void tileReveal(Fl_Widget* widget, void* g) {
	// Callback for tiles with no adjacent mines - causes cascade
	Tile* tile = dynamic_cast<Tile*>(widget);
	Board* game = static_cast<Board*>(g);

	if (!game->timeStart) { // Start timer
		game->timeStart = true;
		Fl::add_timeout(1.0, timeCb, game);
	}

	if (tile->isTileFlag() && !tile->isCascade() && !tile->isUncovered()) { // Place flag image on tile
		tile->image(game->images.at(2));
		--game->flagsLeft;
		string val = to_string(game->flagsLeft);
		game->flagCounts->copy_label(val.c_str());
		if (displayVariables) {
			cout << "Flags Left: " << game->flagsLeft << endl;
		}
		return;
	}
	else if (tile->isTileQuestion() && !tile->isCascade() && !tile->isUncovered()) { // Place question image on tile
		tile->image(game->images.at(3));
		++game->flagsLeft;
		string val = to_string(game->flagsLeft);
		game->flagCounts->copy_label(val.c_str());
		if (displayVariables) {
			cout << "Flags Left: " << game->flagsLeft << endl;
		}
		return;
	}

	if (tile->isCascade() && !tile->isUncovered() && !tile->isTileFlag()) { // simulate tile press
		tile->handle(FL_RELEASE);
		if (tile->getAdjMines() != 0) {
			return;
		}
	}

	

	if (!tile->isUncovered() && !tile->isTileFlag()) { // Cascade tiles around current empty tile using recursion
		tile->setUncovered(true);
		tile->setCascade(false);
		game->incTilesClicked();
		if ((tile->top != NULL) && !tile->top->isUncovered() && !(tile->top->isTileFlag() || tile->top->isTileQuestion())) { // cascade top middle tile
			tile->top->setCascade(true);
			tileReveal(tile->top, game);
		}
		if ((tile->topLeft != NULL) && !tile->topLeft->isUncovered() && !tile->topLeft->isTileFlag()) { // cascade top left tile
			tile->topLeft->setCascade(true);
			tileReveal(tile->topLeft, game);
		}
		if ((tile->topRight != NULL) && !tile->topRight->isUncovered() && !tile->topRight->isTileFlag()) { // cascade top right tile
			tile->topRight->setCascade(true);
			tileReveal(tile->topRight, game);
		}
		if ((tile->left != NULL) && !tile->left->isUncovered() && !tile->left->isTileFlag()) { // cascade left middle tile
			tile->left->setCascade(true);
			tileReveal(tile->left, game);
		}
		if ((tile->right != NULL) && !tile->right->isUncovered() && !tile->right->isTileFlag()) { // cascade right tile
			tile->right->setCascade(true);
			tileReveal(tile->right, game);
		}
		if ((tile->bottom != NULL) && !tile->bottom->isUncovered() && !tile->bottom->isTileFlag()) { // cascade bottom middle tile
			tile->bottom->setCascade(true);
			tileReveal(tile->bottom, game);
		}
		if ((tile->bottomLeft != NULL) && !tile->bottomLeft->isUncovered() && !tile->bottomLeft->isTileFlag()) { // cascade bottom left tile
			tile->bottomLeft->setCascade(true);
			tileReveal(tile->bottomLeft, game);
		}
		if ((tile->bottomRight != NULL) && !tile->bottomRight->isUncovered() && !tile->bottomRight->isTileFlag()) { // cascade bottom right tile
			tile->bottomRight->setCascade(true);
			tileReveal(tile->bottomRight, game);
		}
	}

	game->setGameEnd(game->checkIfWon()); // Check if game ended/won
	if (displayFunctionCalls) {
		if (game->checkGameEnd()) {
			cout << "Game Won!!!" << endl << endl;
		}
	}
}

void tileMineReveal(Fl_Widget* widget, void* g) {
	// Callback for tiles with mines - runs when a mine is triggered
	Tile* tile = dynamic_cast<Tile*>(widget);
	Board* game = static_cast<Board*>(g);

	if (!game->timeStart) { // Start timer
		game->timeStart = true;
		Fl::add_timeout(1.0, timeCb, game);
	}

	if (!tile->isTileFlag()) { // Place mine clicked image on tile
		tile->deimage(game->images.at(1));
	}

	if (tile->isTileFlag() && !tile->isCascade()) { // Places flag image on tile
		tile->image(game->images.at(2));
		--game->flagsLeft;
		string val = to_string(game->flagsLeft);
		game->flagCounts->copy_label(val.c_str());
		if (displayVariables) {
			cout << "Flags Left: " << game->flagsLeft << endl;
		}
		return;
	}
	else if (tile->isTileQuestion() && !tile->isCascade()) { // Places question image on tile
		tile->image(game->images.at(3));
		++game->flagsLeft;
		string val = to_string(game->flagsLeft);
		game->flagCounts->copy_label(val.c_str());
		if (displayVariables) {
			cout << "Flags Left: " << game->flagsLeft << endl;
		}
		return;
	}

	for (int i = 0; i < game->getWidth(); ++i) {
		for (int j = 0; j < game->getHeight(); ++j) {
			if (game->tiles.at(i).at(j)->isTileMine()) { // Reveal mines
				game->tiles.at(i).at(j)->label(0);
				game->tiles.at(i).at(j)->value(1);
				game->tiles.at(i).at(j)->deactivate();
			}
			else {
				if (game->tiles.at(i).at(j)->isTileFlag()) { // Place mine-wrong image on tiles that were flagged but are not mines
					game->tiles.at(i).at(j)->value(1);
					game->tiles.at(i).at(j)->deimage(game->images.at(4));
				}
				game->tiles.at(i).at(j)->deactivate(); // Deactivate all other tiles to prevent further presses
			}
		}
	}

	game->window->color(FL_DARK_RED); // Make window red, indicating game lost
	game->window->redraw();
	if (displayFunctionCalls) {
		cout << "You Lost :(" << endl << endl;
	}
	game->setGameEnd(true);
	game->timeStart = false; // stop timer
}

void setCallbacks(Board& g) {
	// Function to assign correct callback for each tile
	for (int i = 0; i < g.getWidth(); ++i) {
		for (int j = 0; j < g.getHeight(); ++j) {
			if ((g.tiles.at(i).at(j)->getAdjMines() == 0) && !(g.tiles.at(i).at(j)->isTileMine())) {
				g.tiles.at(i).at(j)->callback(tileReveal, &g);
			}
			else if (g.tiles.at(i).at(j)->isTileMine()) {
				g.tiles.at(i).at(j)->callback(tileMineReveal, &g);
			}
			else {
				g.tiles.at(i).at(j)->callback(tileDefaultCallback, &g);
			}
		}
	}
}

void beginGame(Board& g) {
	// Start new game - reset callbacks, timer, flag counter, images
	string val = to_string(g.getMines());
	g.flagCounts->copy_label(val.c_str());
	g.timer->copy_label("0");
	setTileImages(g);
	setCallbacks(g);
	g.t = 0;
	g.timeStart = false;
	if (displayVariables) {
		cout << "Seed: " << g.getSeedNum() << endl;
	}
	if (displayFunctionCalls) {
		cout << "Starting New Game..." << endl << endl;
	}
}

void clearBoard(Board* game, Fl_Window* window) {
	// Clear previous tiles from window to start new game
	if (displayFunctionCalls) {
		cout << endl << "Clearing Window..." << endl;
	}

	for (int i = 0; i < game->getWidth(); ++i) {
		for (int j = 0; j < game->getHeight(); ++j) {
			Fl::delete_widget(game->tiles.at(i).at(j));
		}
	}
	window->color(FL_BACKGROUND_COLOR);
	window->redraw();
}

void restartCallback(Fl_Widget* widget, void* g) {
	// Callback for new game
	Board* game = static_cast<Board*>(g);
	
	clearBoard(game, game->window);
	game->window->begin();
	game->newGame();
	beginGame(*game);
	game->window->end();
}

void newCustomCb1(Fl_Widget* widget, void* g) {
	// Callback for custom game (# of rows) input
	Fl_Int_Input* input = dynamic_cast<Fl_Int_Input*>(widget);
	Board* game = static_cast<Board*>(g);
	int h = 0;
	stringstream c(input->value());
	c >> h;

	if (h <= 1) { // Lower bound for # of rows
		if (displayVariables) {
			cout << "Number to low" << endl;
		}
		h = 2;
	}
	if (h > 49) { // Upper bound
		if (displayVariables) {
			cout << "Number to high" << endl;
		}
		h = 2;
	}

	if (displayFunctionCalls) {
		cout << "Rows Changed" << endl;
	}
	if (displayVariables) {
		cout << "Rows: " << h << endl;
	}

	// Clear previous game, start new game
	clearBoard(game, game->window);
	game->setDificulty(CUSTOM, game->getWidth(), h, game->getMines());
	game->window->begin();
	game->newGame();
	beginGame(*game);
	game->window->end();

	// Resize window
	game->window->resize(game->window->x_root(), game->window->y_root(), game->window->w(), 150 + 18 * h);
}

void newCustomCb2(Fl_Widget* widget, void* g) {
	// Callback for custom game (# of columns) input
	Fl_Int_Input* input = dynamic_cast<Fl_Int_Input*>(widget);
	Board* game = static_cast<Board*>(g);
	int w = 0;
	stringstream c(input->value());
	c >> w;

	if (w <= 1) { // Lower bound for columns
		if (displayVariables) {
			cout << "Number to low" << endl;
		}
		w = 2;
	}
	if (w > 98) { // Upper bound
		if (displayVariables) {
			cout << "Number to high" << endl;
		}
		w = 2;
	}

	if (displayFunctionCalls) {
		cout << "Columns Changed" << endl;
	}
	if (displayVariables) {
		cout << "Columns: " << w << endl;
	}

	// Clear previous game, start new game
	clearBoard(game, game->window);
	game->setDificulty(CUSTOM, w, game->getHeight(), game->getMines());
	game->window->begin();
	game->newGame();
	beginGame(*game);
	game->window->end();

	// Resize window/widgets
	game->window->resize(game->window->x_root(), game->window->y_root(), 70 + 18 * w, game->window->h());
	game->restart->position((game->window->w()) / 2 - 17, 60);
	game->menuBar->resize(game->menuBar->x(), game->menuBar->y(), game->window->w(), 30);
	game->timer->position(game->window->w() - 67, 40);
}

void newCustomCb3(Fl_Widget* widget, void* g) {
	// Callback for custom game (# of mines) input
	Fl_Int_Input* input = dynamic_cast<Fl_Int_Input*>(widget);
	Board* game = static_cast<Board*>(g);
	int m = 0;
	stringstream c(input->value());
	c >> m;

	if (m > (game->getWidth()) * (game->getHeight())) {
		if (displayVariables) {
			cout << "Number too High" << endl;
		}
		m = game->getWidth() * game->getHeight();
	}

	if (displayFunctionCalls) {
		cout << "Number of Mines Changed" << endl;
	}
	if (displayVariables) {
		cout << "Mines: " << m << endl;
	}

	// Clear previous game, start new game
	clearBoard(game, game->window);
	game->setDificulty(CUSTOM, game->getWidth(), game->getHeight(), m);
	game->window->begin();
	game->newGame();
	beginGame(*game);
	game->window->end();
}

void difficultyCb1(Fl_Widget* widget, void* g) {
	// Callback for when beginner difficulty is selected
	Board* game = static_cast<Board*>(g);
	
	if (displayFunctionCalls) {
		cout << "Beginner Game Chosen" << endl;
	}
	if (displayVariables) {
		cout << "Columns: 9" << endl;
		cout << "Rows: 9:" << endl;
		cout << "Mines: 10" << endl;
	}

	// Clear previous game, begin new game
	clearBoard(game, game->window);
	game->window->begin();
	game->setDificulty(BEGINNER);
	game->newGame();
	beginGame(*game);
	game->window->end();

	// Resize window/widgets
	game->window->resize(game->window->x_root(), game->window->y_root(), 232, 312);
	game->restart->position(99, 60);
	game->menuBar->resize(game->menuBar->x(), game->menuBar->y(), 232, 30);
	game->timer->position(165, 40);
}

void difficultyCb2(Fl_Widget* widget, void* g) {
	// Callback for when intermediate difficulty is selected
	Board* game = static_cast<Board*>(g);

	if (displayFunctionCalls) {
		cout << "Intermediate Game Chosen" << endl;
	}
	if (displayVariables) {
		cout << "Columns: 16" << endl;
		cout << "Rows: 16:" << endl;
		cout << "Mines: 40" << endl;
	}


	// Clear previous game, start new game
	clearBoard(game, game->window);
	game->window->begin();
	game->setDificulty(INTERMEDIATE);
	game->newGame();
	beginGame(*game);
	game->window->end();

	// Resize window/widgets
	game->window->resize(game->window->x_root(), game->window->y_root(), 358, 438);
	game->restart->position(162, 60);
	game->menuBar->resize(game->menuBar->x(), game->menuBar->y(), 358, 30);
	game->timer->position(291, 40);
}

void difficultyCb3(Fl_Widget* widget, void* g) {
	// Callback for when expert difficulty is selected
	Board* game = static_cast<Board*>(g);

	if (displayFunctionCalls) {
		cout << "Expert Game Chosen" << endl;
	}
	if (displayVariables) {
		cout << "Columns: 30" << endl;
		cout << "Rows: 16:" << endl;
		cout << "Mines: 99" << endl;
	}

	// Clear previous game, start new game
	clearBoard(game, game->window);
	game->window->begin();
	game->setDificulty(EXPERT);
	game->newGame();
	beginGame(*game);
	game->window->end();

	// Resize window/widgets
	game->window->resize(game->window->x_root(), game->window->y_root(), 610, 438);
	game->restart->position(288, 60);
	game->menuBar->resize(game->menuBar->x(), game->menuBar->y(), 610, 30);
	game->timer->position(543, 40);
}

void difficultyCb4(Fl_Widget* widget, void* g) {
	// Callback for when custom game is selected
	Board* game = static_cast<Board*>(g);
	Fl_Menu_Window* menuWindow = new Fl_Menu_Window(game->window->x(), game->window->y(), 150, 100);
	Fl_Int_Input* input;

	if (displayFunctionCalls) {
		cout << "Custom Game Chosen" << endl;
	}

	// Create window for inputs
	input = new Fl_Int_Input(70, 10, 60, 20, "Rows");
	input->when(FL_WHEN_CHANGED);
	input->callback(newCustomCb1, game);
	input->tooltip("Max: 49 Rows, Min: 2 Rows"); // Any # larger than 49 makes window >1080p vertically

	input = new Fl_Int_Input(70, 40, 60, 20, "Columns");
	input->when(FL_WHEN_CHANGED);
	input->callback(newCustomCb2, game);
	input->tooltip("Max: 98 Columns, Min: 2 Columns"); // Any # larger than 98 makes window >1920p horizontally

	input = new Fl_Int_Input(70, 70, 60, 20, "Mines");
	input->when(FL_WHEN_CHANGED);
	input->callback(newCustomCb3, game);
	input->tooltip("Max: Rows * Columns"); // Max # of mines is equal to total # of tiles

	menuWindow->end();
	menuWindow->show();
}

void showMines(Fl_Widget* widget, void* g) {
	// Debug function for showing mine locations - callback
	Board* game = static_cast<Board*>(g);

	if (!showMineLocations) { // Display mines
		game->setDebugMines(true);
		showMineLocations = true;
		if (!game->checkGameEnd()) {
			for (int i = 0; i < game->getWidth(); ++i) {
				for (int j = 0; j < game->getHeight(); ++j) {
					if (game->tiles.at(i).at(j)->isTileMine()) {
						game->tiles.at(i).at(j)->image(0);
						game->tiles.at(i).at(j)->labelcolor(FL_DARK_RED);
						game->tiles.at(i).at(j)->copy_label("M");
					}
				}
			}
		}
		if (displayFunctionCalls) {
			cout << "Showing Mine Locations..." << endl;
		}
	}
	else { // Hide mines
		showMineLocations = false;
		game->setDebugMines(false);
		for (int i = 0; i < game->getWidth(); ++i) {
			for (int j = 0; j < game->getHeight(); ++j) {
				if (game->tiles.at(i).at(j)->isTileMine()) {
					game->tiles.at(i).at(j)->label(0);
				}
			}
		}
		if (displayFunctionCalls) {
			cout << "Hiding Mine Locations..." << endl;
		}
	}
}

void sameGameCallback(Fl_Widget* widget, void* g) {
	// Callback to start a game with same mine locations
	Board* game = static_cast<Board*>(g);
	int seed = game->getSeedNum();

	if (displayFunctionCalls) {
		cout << "Same Game Chosen" << endl;
	}

	// Clear previous game, start new game
	clearBoard(game, game->window);
	game->window->begin();
	game->newGame(seed);
	beginGame(*game);
	game->window->end();
}

void displayFunctions(Fl_Widget* widget, void* g) {
	// Debug function to show function calls in console
	if (!displayFunctionCalls) { // Turn on debugging
		displayFunctionCalls = true;
		cout << "Display Calls: On" << endl;
	}
	else { // Turn off
		displayFunctionCalls = false;
		cout << "Display Calls: Off" << endl;
	}
}

void displayVariableVal(Fl_Widget* widget, void* g) {
	// Debug function to display variables in console
	if (!displayVariables) { // Turn on
		displayVariables = true;
		cout << "Display Variables: On" << endl;
	}
	else { // Turn off
		displayVariables = false;
		cout << "Display Variables: Off" << endl;
	}
}

void infoWindow(Fl_Widget* widget, void* g) {
	// About developer window
	Board* game = static_cast<Board*>(g);
	Fl_Menu_Window* infoWindow = new Fl_Menu_Window(game->window->x(), game->window->y(), 175, 225);
	Fl_Box* imgBox;
	Fl_Box* infoBox;

	imgBox = new Fl_Box(24, 20, 128, 134);
	imgBox->image(game->images.at(5));
	infoBox = new Fl_Box(82, 185, 10, 10, "Ray Santana\nElectrical Engineering\nSophomore");
	
	infoWindow->end();
	infoWindow->show();
}

void makeMenu(Board& g, int w) {
	// Function to create menu for window
	Fl_Menu_Bar* menuBar = new Fl_Menu_Bar(0, 0, 232, 30);
	g.menuBar = menuBar;
	menuBar->add("Game/New Game", 0, restartCallback, &g, 0);
	menuBar->add("Game/Same Game", 0, sameGameCallback, &g, 0);
	menuBar->add("Game/Difficulty/Beginner", 0, difficultyCb1, &g, FL_MENU_RADIO);
	menuBar->add("Game/Difficulty/Intermediate", 0, difficultyCb2, &g, FL_MENU_RADIO);
	menuBar->add("Game/Difficulty/Expert", 0, difficultyCb3, &g, FL_MENU_RADIO);
	menuBar->add("Game/Difficulty/Custom", 0, difficultyCb4, &g, FL_MENU_RADIO);
	menuBar->add("Game/Debug/Show Mines ", 0, showMines, &g, FL_MENU_TOGGLE);
	menuBar->add("Game/Debug/Show Function Calls ", 0, displayFunctions, &g, FL_MENU_TOGGLE);
	menuBar->add("Game/Debug/Show Variables", 0, displayVariableVal, &g, FL_MENU_TOGGLE);
	menuBar->add("About/Developer", 0, infoWindow, &g);
}

int main() {
	Fl_Window* window = new Fl_Window(232, 312, "Minesweeper");
	Fl_Button* restart = new Fl_Button(99, 60, 34, 34);
	Fl_JPEG_Image restartImage("images/smiley.jpg");
        Fl_Box* flagCounter = new Fl_Box(22, 40, 45, 25, "");
	Fl_Box* clock = new Fl_Box(165, 40, 45, 25, "0");
	Board game;

	clock->box(FL_DOWN_BOX);
	clock->color(FL_DARK1);
	clock->labelcolor(FL_RED);
	clock->labeltype(FL_NORMAL_LABEL);
	clock->labelsize(16);
	clock->labelfont(1);

	flagCounter->box(FL_DOWN_BOX);
	flagCounter->color(FL_DARK1);
	flagCounter->labelcolor(FL_RED);
	flagCounter->labeltype(FL_NORMAL_LABEL);
	flagCounter->labelsize(16);
	flagCounter->labelfont(1);

	game.window = window; // Add window pointer to board object
	game.restart = restart; // Add new-game button pointer to board object
	game.flagCounts = flagCounter; // Add flagcounter pointer to board object
	game.timer = clock; // Add timer pointer to board object
	getImages(game.images); // Get needed images
	makeMenu(game, window->w() + 10); // Call function to make menu for window
	restart->image(restartImage); // Place smiley face image :)
	restart->callback(restartCallback, &game); 
	beginGame(game); // Begin first game

	window->end();
	window->show();
	return Fl::run();
} 
