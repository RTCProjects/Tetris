CFLAGS = -g -Wall -o
LFLAGS =  `pkg-config --cflags --libs gtk+-3.0`
CC = g++

C_INCLUDES = -Iinc/


.PHONY: run-main

run-main: all
	./main

all:
	$(CC) -rdynamic $(CFLAGS) main src/main.cpp src/logic.cpp src/vector.cpp src/figure.cpp $(C_INCLUDES) $(LFLAGS) 