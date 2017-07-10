CC = g++
CFLAGS = -g
LDFLAGS = `pkg-config --libs opencv`

OBJS = main.cpp VideoWrapper.cpp filters.cpp pixelSort/pixelSort.cpp

OBJ_NAME = ve

all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(OBJ_NAME) 
