CXX = g++

CXXFLAGS = -lglut -lGLU -lGL -lstdc++

main: main.o game.o
	$(CXX) $(CXXFLAGS) -o main main.o game.o

main.o: main.cpp game.h
	$(CXX) $(CXXFLAGS) -c main.cpp
