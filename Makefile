all: enderbin

enderbin: enderbin.c
	gcc enderbin.c -o ender -Wall -Wextra -Werror