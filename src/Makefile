BIN_DIR = "../bin"
INCLUDE_DIR = "../include"
LIB_HEADER = "../libDynamicArray/include"
LIB_COMP = "../bin/lib"

APP_NAME  = "simulator"
LIB_NAME = dynamicArray

CC = gcc

LINK_LIBS = $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags) \
			-lpthread -lallegro_ttf -l$(LIB_NAME)

SRC_FILES = $(APP_NAME).c schedule.c UI.c objects.c

buildStatic:
		$(CC) -L $(LIB_COMP) -I $(INCLUDE_DIR) -I $(LIB_HEADER) $(SRC_FILES) -o $(BIN_DIR)/$(APP_NAME) $(LINK_LIBS)

buildDynamic:
		$(CC) -L $(LIB_COMP) -I $(INCLUDE_DIR) -I $(LIB_HEADER) $(SRC_FILES) -o $(BIN_DIR)/$(APP_NAME) $(LINK_LIBS) -Bdynamic $(LIB_COMP)/lib$(LIB_NAME).so
		export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
