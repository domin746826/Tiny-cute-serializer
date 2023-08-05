all: build
	./main

build:
	g++ -o main main.cpp src/*.cpp
