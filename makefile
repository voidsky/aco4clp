CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

main: main.cpp acoclp.o
	$(CC) $(LFLAGS) -o acoclp acoclp.o main.cpp

acoclp.o: acoclp.cpp acoclp.h
	$(CC) $(CFLAGS) acoclp.cpp

clean:
	rm *.o acoclp
