all: enderbin

enderbin: enderbin.c
	gcc enderbin.c -o ender -O3 -Wall -Wextra -Werror