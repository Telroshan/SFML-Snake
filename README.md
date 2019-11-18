# SFML Snake

A simple snake game made using the [SFML library](https://www.sfml-dev.org/).

[![Video](http://img.youtube.com/vi/dlaFbrp34R8/0.jpg)](http://www.youtube.com/watch?v=dlaFbrp34R8 "SFML Snake video") (Gameplay video)

## Download
You can download the built game from the [releases](https://github.com/Telroshan/SFML-Snake/releases/tag/1.0.0).

_(Note : the downloadable build only targets Windows x64, cf the [dev environment part](https://github.com/Telroshan/SFML-Snake#dev-environment))_

# The game
Eat and grow until your inevitable death, and try to beat the high score !

The movement speed increases each time you eat a fruit, until it reaches the maximum of 2 (initial speed is 1).

Fruits spawn on random free locations in the grid.

![image](https://user-images.githubusercontent.com/19146183/69097899-cf9fa480-0a57-11ea-9974-9af160d741e3.png)

## Controls
WASD / ZQSD / Arrows to change the snake's direction

## External assets credits
- Font : [Press Start 2P](https://www.dafont.com/fr/press-start-2p.font)
- Icon : [Flaticon](https://www.flaticon.com/free-icon/snake_427533)
- Menu music : [FesliyanStudios](https://www.fesliyanstudios.com/royalty-free-music/download/8-bit-presentation/383)
- Game music : [FesliyanStudios](https://www.fesliyanstudios.com/royalty-free-music/download/8-bit-menu/287)
- Sounds : [Freesound - LittleRobotSoundFactory](https://freesound.org/people/LittleRobotSoundFactory/packs/16681/)

# Context
A warm-up project to handle the SFML library, with a simple architecture.

I could add a lot there (by first of all adding sprites and vfx), but I'm not likely going to spend more time on this project.

# Dev environment
- SFML [2.5.1](https://www.sfml-dev.org/download/sfml/2.5.1)
- 64 bit
- Visual Studio 2019
- The SFML is also [statically linked](https://www.sfml-dev.org/tutorials/2.5/start-vc.php)

_(Note : the required headers, libs and dlls are on this repo, and relatively linked in the visual studio project, thus you shouldn't need to setup anything to get it working if you're in the same environment ; otherwise, just adapt to whatever environment you're in !)_

# License
This repo is [MIT licensed](https://github.com/Telroshan/SFML-Snake/blob/master/LICENSE).
