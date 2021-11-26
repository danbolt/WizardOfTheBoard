
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define NUMBER_OF_BOARD_CELLS (BOARD_WIDTH * BOARD_HEIGHT)

#define MAX_NUMBER_OF_INGAME_PIECES 24

#define MAX_NUMBER_OF_INGAME_MONSTERS 8
#define NUMBER_OF_PLAYERS 1
#define NUMBER_OF_INGAME_ENTITIES (NUMBER_OF_PLAYERS + MAX_NUMBER_OF_INGAME_MONSTERS)

#define NUMBER_OF_PROJECTILES 16

#define MONSTER_START_INDEX 1

#define MAX_NUMBER_OF_PUZZLE_SPACES 8

#define MONSTER_TYPE_OGRE 0
#define MONSTER_TYPE_TOAD 1
#define MONSTER_TYPE_SNAKE 2
#define MONSTER_TYPE_JUMPER 3
#define MONSTER_TYPE_SHADOQUEEN 4

#define DEFAULT_STICK_DEADZONE 7

#define STICK_MAX_HORIZONTAL_RANGE 70
#define STICK_MAX_VERTICAL_RANGE 65

#endif