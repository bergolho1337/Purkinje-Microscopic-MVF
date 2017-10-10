# ========================================================================
# Template to compile a C++ code 
# ========================================================================

CC = g++
CFLAGS = -std=c++11 -O2 -c -Wall -Wno-unused-result
PNAME = genPlot

all: $(PNAME)

$(PNAME): $(PNAME).o
	$(CC) -o $(PNAME) $(PNAME).o

$(PNAME).o: $(PNAME).cpp
	$(CC) $(CFLAGS) $(PNAME).cpp

clean:
	rm -rf *o $(PNAME)
