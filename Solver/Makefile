INCLUDE_DIR = include
SOURCE_DIR = src
OBJECT_DIR = obj
CC = g++
CFLAGS = -Wall -Wno-write-strings -O2 -I$(INCLUDE_DIR)
CC_SOURCES = $(wildcard src/*)
STRING_OBJ_AUX = $(CC_SOURCES:.cpp=.o)
STRING_OBJ = $(subst src/,,$(STRING_OBJ_AUX))
CC_OBJ = $(patsubst %,$(OBJECT_DIR)/%,$(STRING_OBJ))
PROGRAM_NAME = purkinjeMVF

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(CC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $< -lm

clean:
	rm -f $(OBJECT_DIR)/*.o $(PROGRAM_NAME)
	rm -f VTK/*.vtk

clcResults:
	rm -r Resultados/Mesh_*

print-%  : ; @echo $* = $($*)
