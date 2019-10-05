all: enderbin

enderbin: enderbin.c
	gcc enderbin.c -o ender -03 -Wall -Wextra -Werror