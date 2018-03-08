#Makefile
CFlags = -std=c++11 -g -I. -static-libgcc -static-libstdc++
Warnings = -Wall -Wextra -Wdouble-promotion -Wswitch-default -Wfloat-equal -Wconversion -Wuseless-cast
#Targets = main.cpp Matrix.cpp NeuralNet.cpp Player.cpp TicTacToe.cpp Genetic.cpp
Targets = main.cpp Matrix.cpp NeuralNet.cpp Player.cpp TicTacToe.cpp

all: $(Targets)
	$(RM) a
	g++ $(CFlags) $(Warnings) $(Targets)
fast: $(Targets)
	$(RM) a
	g++ $(CFlags) $(Warnings) -O3 $(Targets)
clean: 
	$(RM) a
