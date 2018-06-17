#Makefile
CompilerFlags = -std=c++11 -I. -static-libgcc -static-libstdc++
Warnings = -Wall -Wextra -Wdouble-promotion -Wswitch-default -Wfloat-equal \
 -Wconversion -Wuseless-cast -Wshadow
Sanitize = -fsanitize=address -fsanitize=undefined
TestingFlags = -Og -g
ReleaseFlags = -O3
Targets = main.cpp Matrix.cpp NeuralNet.cpp Player.cpp Genetic.cpp


all: $(Targets)
	$(RM) a
	g++ $(CompilerFlags) $(Warnings) $(TestingFlags) $(Targets)
run: $(Targets)
	$(RM) a
	g++ $(CompilerFlags) $(Warnings) $(TestingFlags) $(Targets)
	./a.exe
fast: $(Targets)
	$(RM) a
	g++ $(CompilerFlags) $(Warnings) $(ReleaseFlags) $(Targets)
clean: 
	$(RM) a
