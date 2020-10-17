BIN_DIR = "../bin"
INCLUDE_DIR = "../include"
LIB_HEADER = "../libDynamicArray/include"
LIB_COMP = "../bin/lib"

APP_NAME  = "simulator"
LIB_NAME = dynamicArray

CFLAGS = $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags) -lpthread

buildStatic:
		gcc -L $(LIB_COMP) -I $(INCLUDE_DIR) -I $(LIB_HEADER) $(APP_NAME).c objects.c -o $(BIN_DIR)/$(APP_NAME) $(CFLAGS) -l$(LIB_NAME)

buildDynamic:
		gcc -L $(LIB_COMP) -I $(INCLUDE_DIR) -I $(LIB_HEADER) $(APP_NAME).c objects.c -o $(BIN_DIR)/$(APP_NAME) $(CFLAGS) -Bdynamic $(LIB_COMP)/lib$(LIB_NAME).so
		export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
