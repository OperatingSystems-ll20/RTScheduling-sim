#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <allegro5/allegro5.h>
#include <pthread.h>
#include <consts.h>

enum TILE_TYPE {WALL=0, PATH};
enum DIRECTION {LEFT = 0, RIGHT, UP, DOWN};

/**
 * @brief Used for the insertion of new martians with the UI
 * 
 */
typedef struct {
    int energy;
    int period;
} NewMartian;

/**
 * @brief This struct holds all the variables needed for 
 *        communication with the UI
 * 
 */
typedef struct {
    int _exit;
    int _showHUD;
    int _showMartians;
    int _showMartianPos;
    int _showSimTime;
    int _newSimulationPopUp;
    int _newMartianMenu;
    int _insertNewMartian;
    int _operationMode;
    int _manualOpMenu;
    int _automaticOpMenu;
    int _martianSpeed;
    int _pause;
    int _startSimulation;
    int _stopSimulation;
    int _simulationFinished;
    int _showStopSimWarning;
    int _showSuccessPopUp;
    int _showReport;
    int _saveReport;
    int _prepareAutomaticSim;
    int _prepareManualSim;
    int _errorPopUp;
    int _schedulingAlgorithm;
    NewMartian _newMartians[MAX_MARTIANS];
} Options;

/**
 * @brief Stores the position and type of the tiles that conforms the maze
 * 
 */
typedef struct MazeTiles{
    int x;
    int y;
    int type;
} MazeTile;

/**
 * @brief Stores the position of the maze image inside the screen
 * 
 */
typedef struct {
    int x0;
    int y0;
    int x1;
    int y1;
} MazeBounds;

/**
 * @brief Holds all the information about a martian thread
 * 
 */
typedef struct Martians{
    int id;
    char title[15];
    int martianState;
    int posX;
    int posY;
    int direction;
    int maxEnergy;
    size_t currentEnergy;
    int period;
    int ready;
    int running;
    int doWork;
    int counter;
    int update;
    int arrivalTime;
    int executed; //EDF
    int periodCounter; //EDF
    int flagExec; //RM fix
    pthread_cond_t cond;
} Martian;

void loadMazeTiles(MazeTile pMazeTiles[MAZE_WIDTH][MAZE_HEIGHT], ALLEGRO_BITMAP* pMazeImg);
Martian *newMartian(int pPosX, int pPosY, int pInitalDirection, int pMaxEnergy, int pPeriod);



#endif
