all: enderbin

enderbin: enderbin.c
	$(CC) enderbin.c -o ender -O3 -Wall -Wextra -Werror