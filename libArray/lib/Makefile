INCLUDE_DIR = ../include
INSTALL_DIR = ../../bin/lib
COMP_NAME = libarray
LIB_NAME = array

#Compiles the library in both versions, static and dynamic
all: static dynamic clean

#Compiles the static library
static: $(LIB_NAME).c
		@mkdir -p $(INSTALL_DIR)
		gcc -I $(INCLUDE_DIR) -c $(LIB_NAME).c -o $(INSTALL_DIR)/$(LIB_NAME).o
		ar rv $(INSTALL_DIR)/$(COMP_NAME).a $(INSTALL_DIR)/$(LIB_NAME).o
		ranlib $(INSTALL_DIR)/$(COMP_NAME).a

#Compiles the dynamic library
dynamic: $(LIB_NAME).c
		@mkdir -p $(INSTALL_DIR)
		gcc -I $(INCLUDE_DIR) -c $(LIB_NAME).c -o $(INSTALL_DIR)/$(LIB_NAME).o
		ld -o $(INSTALL_DIR)/$(COMP_NAME).so $(INSTALL_DIR)/$(LIB_NAME).o -shared

#Deletes the object file
clean:
		@rm -f $(INSTALL_DIR)/$(LIB_NAME).o
		@rm -f $(INSTALL_DIR)/$(COMP_NAME).a
		@rm -f $(INSTALL_DIR)/$(COMP_NAME).so
