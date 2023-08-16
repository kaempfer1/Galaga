# CPPND: Capstone Project - Galaga

This is a Capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

This project is based on the classic arcade game, Galaga, that was developed by Namco and brought to the United States by Midway Games in 1981.  There are numerous websites detailing
the history of the game, including [this](https://strategywiki.org/wiki/Galaga) and [this](http://tips.retrogames.com/gamepage/galaga.html).

The game engine used for this version of Galaga is based on the Snake Game that was provided by Udacity to start this Capstone project.
The Snake Game was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="Galaga.gif"/>

## Galaga File Structure:
1. game.h and game.cpp for establishing the screen update rate and overall process flow => Get keyboard inputs -> update player ship & enemy actions/positions -> render the graphics to the screen
2. controller.h and controller.cpp reads the keyboard inputs (arrow keys, space bar, etc) for the player input
3. ship.h and ship.cpp & enemy.h and enemy.cpp establish the player ship class and the enemy class
4. renderer.h and renderer.cpp primarily handles the rendering of game graphics to the screen using the Simple DirectMedia Layer (SDL) development library
5. The resources subfolder contains files for the original Galaga game sprites (GalagaSprites.png), game sounds (*.wav), and a True Type Font (emulogic.ttf) to simulate the original game text format. 

## Project Rubric Satisfaction
This Galaga project addresses the following rubric points for the Udacity capstone project.
1. This README file
2. Code compiles and runs using the the process specified below
3. Satisfies at least 5 total criteria from the rest of the rubric, specifically:
*  (1) Project demonstrates an understanding of C++ functions and control structures:
    - There are numerous function and control examples, including method functions in ship.cpp at lines 43, 90, etc
*  (2) The project reads data from a file and process the data:
    - Galaga reads data from sprite and font files to make the game graphics in renderer.cpp at lines 51 and 70
*  (3) The project accepts user input and processes the input:
    - Galaga uses keyboard inputs (arrow keys and space bar) to control the player's ship in controller.cpp at lines 5-51
*  (4) The project uses Object Oriented Programming techniques:
    - There are numerous examples of OOP techniques, including the player & missile classes and methods in ship.h at lines 3-30
*  (5) Classes use appropriate access specifiers for class members:
    - There are numerious examples of class member access specifiers, including in ship.h at lines 32-76

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* SDL_ttf >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/SDL2_ttf)
* SDL_image >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/SDL2_image)
* SDL_mixer >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/SDL2_mixer)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Galaga`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
