#Makefile
CFlags =  -std=c++11 -g -Wall -I. -static-libgcc -static-libstdc++
Targets = main.cpp Matrix.cpp NeuralNet.cpp TicTacToe.cpp Genetic.cpp


all: $(Targets)
	$(RM) a
	g++ $(CFlags) $(Targets)
fast: $(Targets)
	$(RM) a
	g++ $(CFlags) -O3 $(Targets)
clean: 
	$(RM) a
