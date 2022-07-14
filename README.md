# SDL2_simple_2D_shooter

A simple 2D space shooter game demo created by pure C and SDL(Simple DirectMedia Layer) with Visual Studio.

[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

## Table of Contents

- [Background](#Background)
- [Exhibition](#Exhibition)
- [Install](#install)
- [Usage](#Usage)
- [Features](#Features)
- [Structure](#Structure)
- [Maintainers](#Maintainers)
- [License](#license)

## Background

The game will be launched with a 1280 x 720 window and a starfield background. Then the **titlescreen scene** will be displayed. Additionally, current displayed scene will be switched between **titlescreen scene** and **highscore scene**. Player can press the **fire(X) key** to enter the **main game scene**.
Inside the **main game scene**, a spaceship sprite which indicates the player will also be shown. The spaceship can moved by using 4 arrow keys: Up, down, left, and right. The spaceship can also fire bullets by holding down the **fire(X)** key.
Enemy red spaceships will spawn from the right and then move to the left. Enemies can be destroyed by player's bullets. Besides, enemies can also fire back thus player should try to avoid their bullets. When enemies are destroyed, they will release point pods and player can collect them to gain score points.
Furthermore, the **highscore table scene** is shown after player's death. If the player has earned a new highscore, he/she will be prompted to enter his/her name. Then the **highscore table scene** is displayed and the game can be played again. Game window can be closed by clicking on the window's close button.

## Exhibition(Needs to be updated)

## Install

I Use C language, [SDL(Simple DirectMedia Layer)](https://www.libsdl.org/) , SDL_mixer and Visual Studio under Windows 10 environment for this project.

[Visual Studio Download](https://visualstudio.microsoft.com/vs/)<br>

[SDL Wiki](http://wiki.libsdl.org/FrontPage)

[SDL_mixer Download](https://github.com/libsdl-org/SDL_mixer)

## Usage

1. Download this repo, open(or unzip and open) the **SDL2_simple_2D_shooter** folder with **Visual Studio**.

2. Set up **SDL** according to [lazyfoo's guide](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/index.php).

3. Set up **SDL_mixer** according to [lazyfoo's guide](https://lazyfoo.net/SDL_tutorials/lesson11/index.php)

4. Build and run the project to play the game.

## Features

## Structure

## Maintainers

[@Yunxiang-Li](https://github.com/Yunxiang-Li).

## License

[MIT license](https://github.com/Yunxiang-Li/SDL2_simple_2D_shooter/blob/main/LICENSE)
