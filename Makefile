BIN_DIR = "./bin"
SRC_DIR = "./src"
LIB_ARRAY_DIR = "./libDynamicArray"
APP_NAME = "simulator"

MAKE_FLAGS = --no-print-directory

all: buildApp

buildArrayLib:
		$(MAKE) $(MAKE_FLAGS) -C $(LIB_ARRAY_DIR) all

buildApp:
		@mkdir -p $(BIN_DIR)
		$(MAKE) $(MAKE_FLAGS) -C $(LIB_ARRAY_DIR) static
		$(MAKE) $(MAKE_FLAGS) -C $(SRC_DIR) buildStatic

clean:
		@$(MAKE) $(MAKE_FLAGS) -C $(LIB_ARRAY_DIR) clean
		@rm -r -f $(BIN_DIR)/$(APP_NAME)
