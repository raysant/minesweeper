# Minesweeper
Minesweeper clone written in C++, with a GUI based on the FLTK library. It was written for my first programming class so it may not be the most elegant or simplistic code. 


## Usage
In order to compile this program, you must have	`g++` and the `FLTK` library installed. Look through the [FLTK repository](https://github.com/fltk/fltk) for installation instructions for your platform.

### Installing FLTK on Arch Linux
The `FLTK` library can be found in the community repository. 

```bash
    $ pacman -S fltk
```
### Compiling and Running

```bash
    $ g++ $(fltk-config --cxxflags) -lfltk -lfltk_images *.cpp -o minesweeper
    $ ./minesweeper
```


## Screenshot
<p align="center"><img src="images/minesweeper.png" /></p>