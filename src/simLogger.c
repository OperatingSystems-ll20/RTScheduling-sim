#include <stdlib.h>
#include <simLogger.h>

int initLogger(){
    _fileLog = fopen("/tmp/tmpLog.txt", "w+");

    if(_fileLog) return 1;
    return 0;  
}

void closeLogger(){
    fclose(_fileLog);
    // remove("/tmp/tmpLog.txt");
}

void logSimEvent(const int pEventType, const int pSecTimer, const char *pEvent){
    fprintf(_fileLog, "[0] [%d-%d] %s\n", pEventType, pSecTimer, pEvent);
    fflush(_fileLog);
}


void logMartianEvent(const int pEventType, const int pMartianId, const int pSecTimer, const char *pEvent){
    fprintf(_fileLog, "[1] [%d-%d-%d] %s\n", pEventType, pMartianId, pSecTimer, pEvent);
    fflush(_fileLog);
}