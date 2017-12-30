INCLUDE_DIR = include
SOURCE_DIR = src
OBJECT_DIR = obj
CC = g++
#PROGRAM_FLAGS = -DPMJ -DVTK -DOUTPUT
#PROGRAM_FLAGS = -DPMJ -DOUTPUT
#PROGRAM_FLAGS = -DVTK -DOUTPUT
#PROGRAM_FLAGS = 
CFLAGS = -Wall -Wno-write-strings -std=c++11 -O2 -I$(INCLUDE_DIR) $(PROGRAM_FLAGS)
CC_SOURCES = $(wildcard src/*)
STRING_OBJ_AUX = $(CC_SOURCES:.cpp=.o)
STRING_OBJ = $(subst src/,,$(STRING_OBJ_AUX))
CC_OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(STRING_OBJ))
PROGRAM_NAME = graphreader

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(CC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $< -lm

clean:
	rm -f $(OBJECT_DIR)/*.o $(PROGRAM_NAME)

remade:
	$(MAKE) clean
	$(MAKE)

print-%  : ; @echo $* = $($*)