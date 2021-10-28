CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

main: main.cpp 
	$(CC) $(LFLAGS) *.cpp -o clp 

clean:
	rm *.o clp
