CC = g++
CFLAGS = -Wall
EXEC_NAME = cppstacklang
INCLUDES = 
LIBS = 
OBJ_FILES = Block.o interpreter.o main.o Token.o WorldState.o
INSTALL_DIR = /usr/bin
BUILD_DIR = ./build

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(BUILD_DIR)/$(EXEC_NAME) $(BUILD_DIR)/*.o $(LIBS)

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/$@ -c $<