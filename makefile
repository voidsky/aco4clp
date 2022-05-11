CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
OMPLAG = -fopenmp

main: main.cpp 
	$(CC) $(LFLAGS) $(OMPLAG) -Ofast *.cpp -o clp 

clean:
	rm *.o clp
