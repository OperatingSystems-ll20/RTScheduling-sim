BIN_DIR = "./bin"
SRC_DIR = "./src"
APP_NAME = "simulator"

all: buildApp

buildApp:
		@mkdir -p $(BIN_DIR)
		$(MAKE) -C $(SRC_DIR) build


clean:
		@rm -r -f $(BIN_DIR)/$(APP_NAME)
