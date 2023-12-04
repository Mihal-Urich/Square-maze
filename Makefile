CC = g++
CPP_FLAGS = -lstdc++ -std=c++17 -pedantic -Wall -Werror -Wextra
LIB_FLAGS = -fPIC -shared
CC_FILE = back/FindPath.cc back/NiceMaze.cc

all: start

start: clean
	$(CC) $(LIB_FLAGS) -o libmaz.so $(CC_FILE)
	python3.11 Front/NewMaze.py

clean:
	@rm -rf libmaz.so
	@rm -rf Front/__pycache__
	