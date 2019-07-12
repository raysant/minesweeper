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

//#include <Windows.h> //uncomment to run on Windows
#include "Tile.h"
#include <FL/Fl.H>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

Tile::Tile(int x, int y, int w, int h, const char *label) : Fl_Button(x, y, w, h, label), isFlag(false), isMine(false), 
	uncovered(false), cascade(false), adjacentMines(0) {}

int Tile::handle(int event) {
	if (event == FL_RELEASE) { // Handle event when a button is released
		bool clickedInsideWidget = (Fl::event_button() == FL_LEFT_MOUSE) && Fl::event_inside(this);
		bool rightClickedInside = (Fl::event_button() == FL_RIGHT_MOUSE) && Fl::event_inside(this);

		if (rightClickedInside) { // Handle right click events
			this->value(0);

			if (!isFlag && !isQuestion) { // Flag tile
				isFlag = true;
				cascade = false;
				this->label(0);
				this->do_callback();
			}
			else if (isFlag && !uncovered) { // Question tile
				isFlag = false;
				isQuestion = true;
				cascade = false;
				this->label(0);
				this->do_callback();
			}
			else if (isQuestion) { // Return tile to normal
				isFlag = false;
				isQuestion = false;
				this->label(0);
				this->image(0);
			}
			return 1;
		}
		else if ((clickedInsideWidget && !uncovered) || (cascade && !uncovered)) { // Handle left click events
			if (isFlag) { // Do nothing when flagged tile is clicked
				this->value(0);
				return 1;
			}
			if (isQuestion) {
				isQuestion = false;
				this->label(0);
				this->image(0);
			}
			if ((Fl::event_inside(this) && !isFlag) || (cascade && !isFlag)) { // Do click action when tile is pressed
				this->set_flag(OUTPUT);

				if (isMine) { // Activate mine
					this->label(0);
					this->deactivate();
				}
				else if (!isMine && (adjacentMines != 0) && !isFlag) { // Show # of adj mines on tile
					stringstream a;
					int colors[8] = { FL_BLUE, FL_DARK_GREEN, FL_RED, FL_DARK_MAGENTA, FL_YELLOW,
						FL_DARK_BLUE, FL_DARK_RED, FL_BLACK };
					a << adjacentMines;
					string adjLabel = a.str();
					this->labelcolor(colors[adjacentMines - 1]);
					this->copy_label((adjLabel.c_str()));
					cascade = false;
				}
				else if (cascade && !isMine && !isFlag) { // Uncover tiles with no adj mines
					this->image(0);
					this->value(1);
					cascade = false;
					return 1;
				}

				this->value(1);
				this->do_callback();
				return 1;
			}
		}
		return 1;
	}
	else { // Handle any other events
		return Fl_Button::handle(event);
	}
}
