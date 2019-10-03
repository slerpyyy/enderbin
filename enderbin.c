#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

const char* flag = "::ENDER::FLAG::\x05";
const int flaglen = 15;

const char* damage[] = {"OUGH!", "UMPFFF!!", "AARGH!"};
const char* death[] = {"KRHHHHHHH", "AAAAAAAAAAAAAAAA!!!!"};

int main(int argc, char* argv[])
{
	srand(time(NULL));

	int health = *(flag + flaglen);

	// wat
	if(argc < 1)
	{
		puts("wat");
		exit(-1);
	}

	if(health < 1)
	{
		remove(argv[0]);
		int i = rand() % 2;
		printf("%s (0 hp)\n", death[i]);
		exit(0);
	}

	int tmp_name_len = strlen(argv[0]) + 12;
	char* tmp_name = (char *)malloc(sizeof(char) * tmp_name_len);
	sprintf(tmp_name, "%s.tmp%08X", argv[0], rand());

	FILE *in_file = fopen(argv[0], "r");
	FILE *out_file = fopen(tmp_name, "w");
	
	if(in_file == NULL || out_file == NULL)
	{
		puts("wat");
		exit(-1);
	}

	int curr = 0;
	int prog = 0;

	while((curr = fgetc(in_file)) != EOF)
	{
		if(prog == flaglen)fputc(health - 1, out_file);
		else fputc(curr, out_file);

		if(curr == *(flag + prog))prog++;
		else prog = 0;
	}

	fclose(in_file);
	fclose(out_file);

	chmod(tmp_name, 0x1ed);
	rename(tmp_name, argv[0]);

	int i = rand() % 3;
	printf("%s (%d hp)\n", damage[i], health);

	return 0;
}