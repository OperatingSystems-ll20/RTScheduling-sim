#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro_primitives.h>
#include <consts.h>
#include <reportGenerator.h>

static int BMP_HEIGHT = 100;
static int LEFT_OFFSET = 20;
static int SECOND_WIDTH = 40;
static int SECOND_HEIGHT = 50;

/**
 * @brief Gets the total time of simulation based on the information logged 
 * 
 * @param pFP   File descriptor of the log file
 * @return int  Total simulation time
 */
static int getTimeSpan(FILE *pFP){
    int ret = 0;
    int startTime, endTime;
    char *line = NULL;
    size_t len = 0;
    regex_t regex;
    regmatch_t match[3];
    regcomp(&regex, REGEX_1, REG_EXTENDED | REG_NEWLINE);

    getline(&line, &len, _fp);
    ret = regexec(&regex, line, 3, match, 0);
    if(!ret){
        line[match[2].rm_eo] = '\0';
        startTime = atoi(line + match[2].rm_so);
    }

    while(getline(&line, &len, _fp) != -1); //Go to last line

    ret = regexec(&regex, line, 3, match, 0);
    if(!ret){
        line[match[2].rm_eo] = '\0';
        endTime = atoi(line + match[2].rm_so);
    }

    rewind(_fp);
    return endTime-startTime;
}

/**
 * @brief Reads a line from the log file and extracts the relevant data, such as
 *        the event type, the martian id and time of the event.
 * 
 * @param pRegex        Regex used to extract the data
 * @param pLine         Line to parse
 * @param pEventType    Reference to the event type
 * @param pMartianId    Reference to the martian id
 * @param pTime         Reference to the time of the event
 * @return int          1->Successful
 *                      0->Error
 */                 
static int parseLine(regex_t *pRegex, char *pLine, int *pEventType, int *pMartianId, int *pTime){
    int ret;
    regmatch_t match[3];
    ret = regexec(pRegex, pLine, 4, match, 0);
    if(!ret){
        char tmpStr[strlen(pLine) + 1];
        strcpy(tmpStr, pLine);
        tmpStr[match[1].rm_eo] = '\0';
        *pEventType = atoi(tmpStr + match[1].rm_so);

        strcpy(tmpStr, pLine);
        tmpStr[match[2].rm_eo] = '\0';
        *pMartianId = atoi(strtok(tmpStr+match[2].rm_so, "-"));
        *pTime = atoi(strtok(NULL, "-"));

        return 1;
    }
    else return 0;
}


/**
 * @brief Creates the bitmap image of the report
 * 
 */
static void createBitmap(){
    _simTime = getTimeSpan(_fp);

    int bmpWidth = (SECOND_WIDTH * _simTime) + 50;

    // al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    _reportBMP = al_create_bitmap(bmpWidth, BMP_HEIGHT);
}


/**
 * @brief Draws the grid of seconds on the report bitmap
 * 
 * @param pFont  Font used to draw the seconds
 */
void createGrid(ALLEGRO_FONT *pFont){
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    for(int i=0; i<=_simTime; i++){
        al_draw_line(
            (i*SECOND_WIDTH), 0, 
            (i*SECOND_WIDTH), SECOND_HEIGHT+10, 
            black, 1
        );
        char tmp[4];
        sprintf(tmp, "%d", i);
        al_draw_text(pFont, black, (i*SECOND_WIDTH), SECOND_HEIGHT+15, ALLEGRO_ALIGN_CENTRE, tmp);
    }
}


/**
 * @brief Saves the bitmap to disk
 * 
 */
void saveReport(){
    al_save_bitmap("report.png", _reportBMP);
}


/**
 * @brief Generates the report of the simulation
 * 
 * @param pColors           DynamicArray of colors of the martians
 * @param pFont             Font to draw text
 * @return ALLEGRO_BITMAP*  Allegro bitmap with the report image
 */
ALLEGRO_BITMAP* generateReport(DynamicArray pColors, ALLEGRO_FONT *pFont){
    _fp = fopen(LOG_FILE_PATH, "r");
    createBitmap();    
    al_set_target_bitmap(_reportBMP);
    al_clear_to_color(al_map_rgb(255, 255, 255));

    char *line = NULL;
    size_t len = 0;
    regex_t regex;
    int ret;

    int event, martianId, time;
    ALLEGRO_COLOR *color;
    regcomp(&regex, REGEX_2, REG_EXTENDED | REG_NEWLINE);

    while(getline(&line, &len, _fp) != -1){
        if(parseLine(&regex, line, &event, &martianId, &time)){
            color = (ALLEGRO_COLOR*)pColors.array[martianId];
            switch(event){
                case MARTIAN_START:
                case MARTIAN_CONTINUE:
                    al_draw_filled_rectangle(
                        (time*SECOND_WIDTH),
                        0,
                        (time*SECOND_WIDTH)+SECOND_WIDTH,
                        SECOND_HEIGHT,
                        *color
                    );
                    break;
            }
        }
    }
    fclose(_fp);
    createGrid(pFont);
    return _reportBMP;
}