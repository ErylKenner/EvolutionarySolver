#Makefile
CompilerFlags = -std=c++11 -I. -static-libgcc -static-libstdc++
Warnings = -Wall -Wextra -Wdouble-promotion -Wswitch-default -Wfloat-equal \
 -Wconversion -Wuseless-cast -Wshadow
Sanitize = -fsanitize=address -fsanitize=undefined
TestingFlags = -Og -g
ReleaseFlags = -O3
Targets = ./src/main.cpp ./src/NeuralNet.cpp ./src/Player.cpp ./src/Genetic.cpp
Include = -isystem d:/MinGW/include/eigen-eigen-5a0156e40feb/


all: $(Targets)
	$(RM) a
	g++ $(CompilerFlags) $(Warnings) $(TestingFlags) $(Targets) $(Include)

run: $(Targets)
	$(RM) a
	g++ $(CompilerFlags) $(Warnings) $(TestingFlags) $(Targets) $(Include)
	./a.exe

fast: $(Targets)
	$(RM) a
	g++ $(CompilerFlags) $(Warnings) $(ReleaseFlags) $(Targets) $(Include)
	
clean: 
	$(RM) a
