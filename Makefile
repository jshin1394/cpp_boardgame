# Lines starting with # are comments

# Some variable definitions to save typing later on
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic 

games: games.o games_function.o
	$(CC) $(CFLAGS) games.o games_function.o -o games


games.o: games.cpp games_header.h
	$(CC) $(CFLAGS) -c games.cpp


test_games: test_games.o games_function.o
	$(CC) $(CFLAGS) test_games.o games_function.o -o test_games


test_games.o: test_games.cpp games_header.h
	$(CC) $(CFLAGS) -c test_games.cpp


games_function.o: games_function.cpp games_header.h
	$(CC) $(CFLAGS) -c games_function.cpp


clean:
	rm -f *.o games test_games
