# SDL2_simple_2D_shooter

This is a simple 2D space shooter game demo created by pure C and SDL(Simple DirectMedia Layer) with Visual Studio.

You can check the video, screenshots/GIFs and more information about this demo from my [personal portfolio website](https://yunxiang-li.github.io./#/game-projects).

[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

## Table of Contents

- [Overview](#Overview)
- [Install](#install)
- [Usage](#Usage)
- [Structure](#Structure)
- [Maintainers](#Maintainers)
- [License](#license)

## Overview

The game will be launched with a 1280 x 720 window and a starfield background. Then the **titlescreen scene** will be displayed. Additionally, current displayed scene will be switched between **titlescreen scene** and **highscore scene**. Player can press the **fire(X) key** to enter the **main game scene**.

Inside the **main game scene**, a spaceship sprite which indicates the player will also be shown. The spaceship can moved by using 4 arrow keys: Up, down, left, and right. The spaceship can also fire bullets by holding down the **fire(X)** key.

Enemy red spaceships will spawn from the right and then move to the left. Enemies can be destroyed by player's bullets. Besides, enemies can also fire back thus player should try to avoid their bullets. When enemies are destroyed, they will release point pods and player can collect them to gain score points.

Furthermore, the **highscore table scene** is shown after player's death. If the player has earned a new highscore, he/she will be prompted to enter his/her name. Then the **highscore table scene** is displayed and the game can be played again. Game window can be closed by clicking on the window's close button.

## Install

I Use C language, [SDL(Simple DirectMedia Layer)](https://www.libsdl.org/) , SDL_mixer, SDL_image and Visual Studio under Windows 10 environment for this project.

[Visual Studio Download](https://visualstudio.microsoft.com/vs/)<br>

[SDL Wiki](http://wiki.libsdl.org/FrontPage)

[SDL_mixer Download](https://github.com/libsdl-org/SDL_mixer)

[SDL_image Download](https://github.com/libsdl-org/SDL_image)

## Usage

1. Download this repo, open(or unzip and open) the **SDL2_simple_2D_shooter** folder with **Visual Studio**.

2. Set up **SDL** according to [lazyfoo's guide](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/index.php).

3. Set up **SDL_mixer** according to [lazyfoo's guide](https://lazyfoo.net/SDL_tutorials/lesson11/index.php)

4. Set up **SDL_image** according to [lazyfoo's guide](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php)

5. Build and run the project to play the game.

## Structure

The whole project contains four main folders, **Exhibitions** folder, **Resources** folder, **include** folder and **src** folder.

1. **Exhibitions** folder contains all screenshots and GIFs needed for exhibition.

2. **src** folder contains altogether **11** source files:

3. **include** folder contains altogether **13** header files:

4. **Resources** folder contains **1** font(.ttf) file, **1** highscore data file(.txt), **9** game objects' image files(.png), **1** music file(.ogg) and **5** sound files(.ogg).

Detailed documentation can be viewed via this [link](https://yunxiang-li.github.io/SDL2_shooter_document/html/).

**1.** `background` : handle all background related behaviors.

**2.** `defs` : include all defined variables here.

**3.** `draw` : contain all draw(render) related features.

**4.** `highscores` : process all features related to highscores.

**5.** `init.h` : handle the initialization and cleanup of the game.

**6.** `input.h` : process all user's input.

**7.** `main.h` : be responsible for the entrance and the main game loop.

**8.** `sound.h` : handle all sound behaviors inside the game.

**9.** `stage.h` : run the actual game logic.

**10.** `structs.h` : contain all custom structs used for the game.

**11.** `text.h` : hold all features about in-game text.

**12.** `title.h` : responsible for all behaviours of titlescreen scene.

**13.** `util.h` : store several utility functions to help create the game (such as math calculation and collision detection).

## Maintainers

[@Yunxiang-Li](https://github.com/Yunxiang-Li).

## License

[MIT license](https://github.com/Yunxiang-Li/SDL2_simple_2D_shooter/blob/main/LICENSE)
