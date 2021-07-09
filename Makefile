CC = g++
FILES = Main.cpp $(wildcard ./**/*.cpp)
CFLAGS = -std=c++14 -c -g3 -Wall -O3 -ICube -ISolver -IUtil
INCLUDE_FLAGS = -ICube -ISolver -IUtil

ifeq ($(OS),Windows_NT)
	RM = cmd \/C del
	LINKER_FLAGS := -lmingw32 -lSDL2main -lSDL2 -LC:/libsdl/lib
	INLCUDE_FLAGS := $(INCLUDE_FLAGS) -IC:/libsdl/include
	TARGET = cube-solver.exe
else
	RM = rm
	TARGET = cube-solver
endif

all: $(FILES) 
	$(CC) $(FILES) $(CFLAGS) $(INCLUDE_FLAGS) && \
	$(CC) *.o -o cube-solver $(LINKER_FLAGS)

clean:
	$(RM) *.o $(TARGET)
