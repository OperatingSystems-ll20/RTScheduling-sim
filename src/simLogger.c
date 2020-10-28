#include <stdlib.h>
#include <consts.h>
#include <simLogger.h>

/**
 * @brief Creates the file where the events of the simulation are logged
 * 
 * @return int File descriptor
 */
int initLogger(){
    _fileLog = fopen("/tmp/tmpLog.txt", "w+");

    if(_fileLog) return 1;
    return 0;  
}

/**
 * @brief Closes the file
 * 
 */
void closeLogger(){
    fclose(_fileLog);
    // remove("/tmp/tmpLog.txt");
}

/**
 * @brief Writes an event to the file
 * 
 * @param pEventType Type of event
 * @param pSecTimer  Time
 * @param pEvent     Description
 */
void logSimEvent(const int pEventType, const int pSecTimer, const char *pEvent){
    fprintf(_fileLog, "[%d] [%d] %s\n", pEventType, pSecTimer, pEvent);
    fflush(_fileLog);
}


/**
 * @brief Writes an event that needs an id identification.
 * 
 * @param pEventType Type the event
 * @param pMartianId Id of the martian that originated the event
 * @param pSecTimer  Time
 * @param pEvent     Description
 */
void logMartianEvent(const int pEventType, const int pMartianId, const int pSecTimer, const char *pEvent){
    fprintf(_fileLog, "[%d] [%d-%d] %s\n", pEventType, pMartianId, pSecTimer, pEvent);
    fflush(_fileLog);
}