BIN_DIR = "./bin"
SRC_DIR = "./src"
LIB_ARRAY_DIR = "./libDynamicArray"
APP_NAME = "simulator"

all: buildApp

buildArrayLib:
		$(MAKE) -C $(LIB_ARRAY_DIR) all

buildApp:
		@mkdir -p $(BIN_DIR)
		$(MAKE) -C $(LIB_ARRAY_DIR) static
		$(MAKE) -C $(SRC_DIR) buildStatic


clean:
		$(MAKE) -C $(LIB_ARRAY_DIR) clean
		@rm -r -f $(BIN_DIR)/$(APP_NAME)
