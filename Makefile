CC = g++
CFLAGS = -g
LDFLAGS = `pkg-config --libs opencv`
UNIT_EXE = ut
MAIN_EXE = ve


all : main unit


MAIN = main.cpp VideoWrapper.cpp filters.cpp pixelSort/pixelSort.cpp cmdParser.cpp controls.cpp erToCube.cpp
main :$(MAIN)
	$(CC) $(MAIN) $(CFLAGS) $(LDFLAGS) -o $(MAIN_EXE) 

UNIT = erToCubeUnit.cpp erToCube.cpp
unit : $(UNIT)
	$(CC) $(UNIT) $(CFLAGS) $(LDFLAGS) -o $(UNIT_EXE) 


