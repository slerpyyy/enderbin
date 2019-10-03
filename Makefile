all: enderbin

enderbin: enderbin.c
	gcc enderbin.c -o enderbin -Wall -Wextra -Werror