

.PHONY : all clean

all : 
	gcc main.c zchess.c -o zchess_test

clean :
	rm -rf zchess_test
