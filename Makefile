CC = g++
FILES = Main.cpp $(wildcard ./**/*.cpp)
CFLAGS = -std=c++14 -c -Wall -ICube -ISolver -IUtil
INCLUDE_FLAGS = -ICube -ISolver -IUtil

ifeq ($(OS),Windows_NT)
	RM = cmd \/C del
	LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -LC:/libsdl/lib
	INLCUDE_FLAGS := $(INCLUDE_FLAGS) -IC:/libsdl/include
	TARGET = cube-solver.exe
else
	RM = rm
	LINKER_FLAGS = -lSDL2main -lSDL2 -L/usr/local/Cellar/sdl2/2.0.14_1/lib
	INCLUDE_FLAGS := $(INCLUDE_FLAGS) -I/usr/local/Cellar/sdl2/2.0.14_1/include
	TARGET = cube-solver
endif

all: debug

debug: $(FILES) 
	$(CC) $(FILES) $(CFLAGS) -g $(INCLUDE_FLAGS) && \
	$(CC) *.o -o cube-solver $(LINKER_FLAGS)

release: $(FILES) 
	$(CC) $(FILES) $(CFLAGS) -O3 $(INCLUDE_FLAGS) && \
	$(CC) *.o -o cube-solver -s $(LINKER_FLAGS)

clean:
	$(RM) *.o $(TARGET)
