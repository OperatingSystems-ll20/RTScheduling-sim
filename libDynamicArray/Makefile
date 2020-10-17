LIB_DIR = ./lib

all: static dynamic

static:
		$(MAKE) -C $(LIB_DIR) static

dynamic:
		$(MAKE) -C $(LIB_DIR) dynamic

#Deletes the object files created during compilation
clean:
	$(MAKE) -C $(LIB_DIR) clean
