#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* flag = "AAAAAAXX\x03";
const int flaglen = 8;

int main(int argc, char* argv[])
{
	int num = *(flag + flaglen);
	printf("num: %d\n", num);

	// change file

	if(argc < 1)
	{
		puts("wat");
		exit(-1);
	}

	FILE *infile = fopen((const char *)argv[0], "r");
	FILE *outfile = fopen("tmp", "w");
	
	if(infile == NULL || outfile == NULL)
	{
		puts("wat");
		exit(-1);
	}

	int curr = 0;
	int prog = 0;

	while((curr = fgetc(infile)) != EOF)
	{
		if(prog == flaglen)
		{
			puts("flag found!!");
			fputc(num + 1, outfile);
		}
		else
		{
			fputc(curr, outfile);
		}

		if(curr == *(flag + prog))prog++;
		else prog = 0;
	}

	fclose(infile);
	fclose(outfile);

	return 0;
}