LIB_DIR = ./lib

MAKE_FLAGS = --no-print-directory

all: static dynamic

static:
		$(MAKE) $(MAKE_FLAGS) -C $(LIB_DIR) static

dynamic:
		$(MAKE) $(MAKE_FLAGS) -C $(LIB_DIR) dynamic

#Deletes the object files created during compilation
clean:
	@$(MAKE) -C $(LIB_DIR) clean
