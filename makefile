all: main.cpp Matrix.cpp NeuralNet.cpp TicTacToe.cpp Genetic.cpp
	g++ -static-libgcc -static-libstdc++ -std=c++11 -Wall -Wno-sign-compare main.cpp Matrix.cpp NeuralNet.cpp TicTacToe.cpp Genetic.cpp
clean: 
	$(RM) a