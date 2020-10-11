BIN_DIR = "../bin"
INCLUDE_DIR = "../include"
APP_NAME  = "simulator"

CFLAGS = $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)

build:
		gcc -I $(INCLUDE_DIR) $(APP_NAME).c objects.c -o $(BIN_DIR)/$(APP_NAME) $(CFLAGS)
