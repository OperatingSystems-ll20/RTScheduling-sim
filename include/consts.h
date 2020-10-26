#ifndef _CONSTS_H
#define _CONSTS_H

#define SCREEN_WIDHT 1024
#define SCREEN_HEIGHT 700
#define MAZE_WIDTH 480
#define MAZE_HEIGHT 480
#define MAX_MARTIANS 8

enum SHEDULING_ALGORITHM { RM, EDF};
enum OPERATION_MODE {MANUAL, AUTOMATIC, UNDEFINED};
enum EVENT_TYPES {SIM_START, SIM_END, MARTIAN_START, MARTIAN_CONTINUE, MARTIAN_TIMER, MARTIAN_END, SCHEDULE_ERROR};

static const double REFRESH_RATE = 30.0;

static const int TILE_SIZE = 32;
static const int MARTIAN_SIZE = 15;
static const int MARTIAN_DEFAULT_SPEED = 6;

static const int MAZE_START_X = 0;
static const int MAZE_START_Y = 13;
static const int MAZE_END_X = 1;
static const int MAZE_END_Y = 0;

static const char *ABOUT_MANUAL_MODE = "* Manual mode allows to add new martians at any time";
static const char *ABOUT_AUTOMATIC_MODE = "* Automatic mode starts the simulation"
    "with a set number of martians";
static const char *ADD_MARTIAN_INFO = "You can add new martians at any moment in"
    " the menu or by pressing the N key";

static const char *LOG_FILE_PATH = "/tmp/tmpLog.txt";
static const char *REGEX_1 = "^\\[([[:digit:]])\\][[:blank:]]*\\[([[:digit:]]*)\\][[:blank:]]*(.*)$";
static const char *REGEX_2 = "^\\[([[:digit:]])\\][[:blank:]]*\\[([[:digit:]]+-[[:digit:]]+)\\]"
    "[[:blank:]]*(.*)$";

#endif
