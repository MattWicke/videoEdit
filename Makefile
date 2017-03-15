CC = g++
CFLAGS = -g
LDFLAGS = `pkg-config --libs opencv`

OBJS = main.cpp VideoWrapper.cpp filters.cpp

OBJ_NAME = vidEdit

all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(OBJ_NAME) 
