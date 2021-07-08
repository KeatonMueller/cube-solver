CC = g++
OBJ = Main.o Util.o Cube.o Move.o Solver.o SolverCross.o SolverF2L.o SolverOLL.o SolverPLL.o 
HEADER = Util/Util.h Cube/Cube.h Cube/Move.h Solver/Solver.h Solver/SolverCross.h Solver/SolverF2L.h Solver/SolverOLL.h Solver/SolverPLL.h
CFLAGS = -std=c++14 -c -Wall -O3 -ICube -ISolver -IUtil

ifeq ($(OS),Windows_NT)
	RM = cmd \/C del
	TARGET = cube-solver.exe
else
	RM = rm
	TARGET = cube-solver
endif

all: cube-solver

cube-solver: $(OBJ)
	$(CC) $(OBJ) -o $@

Main.o: Main.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

Util.o: Util/Util.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

Cube.o: Cube/Cube.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

Move.o: Cube/Move.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

Solver.o: Solver/Solver.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

SolverCross.o: Solver/SolverCross.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

SolverF2L.o: Solver/SolverF2L.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

SolverOLL.o: Solver/SolverOLL.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

SolverPLL.o: Solver/SolverPLL.cpp $(HEADER)
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o $(TARGET)
