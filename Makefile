CC = g++
FILES = $(wildcard ./**/*.cpp)
CFLAGS = -std=c++14 -c -Wall -ICube -ISolver -IUtil
INCLUDE_FLAGS = -ICube -ISolver -IUtil

ifeq ($(OS),Windows_NT)
	RM = cmd \/C del
	TARGET = cube-solver.exe
else
	RM = rm
	TARGET = cube-solver
endif

all: debug

debug: Main.cpp $(FILES) 
	$(CC) Main.cpp $(FILES) $(CFLAGS) -g $(INCLUDE_FLAGS) && \
	$(CC) *.o -o cube-solver

release: Main.cpp $(FILES) 
	$(CC) Main.cpp $(FILES) $(CFLAGS) -O3 $(INCLUDE_FLAGS) && \
	$(CC) *.o -o cube-solver -s

web: Web.cpp $(FILES) 
	source /Users/keaton/work/emsdk/emsdk_env.sh && \
	em++ Web.cpp $(FILES) -c $(INCLUDE_FLAGS) && \
	em++ --bind -o cube-solver.js *.o -s ALLOW_MEMORY_GROWTH=1

clean:
	$(RM) *.o $(TARGET)
