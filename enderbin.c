#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


const char* flag = "$::ENDER::FLAG::\x05";
const int flag_len = 16;

const char* damage[] = {"OUGH!", "UMPFFF!!", "AARGH!"};
const int damage_len = 3;

const char* death[] = {"KRHHHHHHH", "AAAAAAAAAAAAAAAA!!!!"};
const int death_len = 2;



int main(int argc, char* argv[])
{
	int health = *(flag + flag_len);
	
	srand(time(NULL) - health);

	if(argc < 1)
	{
		fprintf(stderr, "Something went horribly wrong\n");
		exit(-1);
	}

	if(health < 1)
	{
		remove(argv[0]);
		int death_index = rand() % death_len;
		printf("%s (0 hp)\n", death[death_index]);
		exit(0);
	}



	int tmp_name_len = strlen(argv[0]) + 12;
	char* tmp_name = (char *)malloc(sizeof(char) * tmp_name_len);
	sprintf(tmp_name, "%s.tmp%08X", argv[0], rand());



	FILE *in_file = fopen(argv[0], "r");
	if(in_file == NULL)
	{
		fprintf(stderr, "Could not open source file\n");
		exit(-1);
	}

	FILE *out_file = fopen(tmp_name, "w");
	if(out_file == NULL)
	{
		fprintf(stderr, "Could not write to temp file\n");
		exit(-1);
	}



	int curr = 0;
	int prog = 0;

	while((curr = fgetc(in_file)) != EOF)
	{
		if(prog == flag_len)fputc((char)(health-1), out_file);
		else fputc(curr, out_file);

		if(curr == *(flag + prog))prog++;
		else if(curr == *flag)prog = 1;
		else prog = 0;
	}

	fclose(in_file);
	fclose(out_file);



	if(chmod(tmp_name, 0x1ed))
	{
		remove(tmp_name);
		fprintf(stderr, "Could not chmod temp file\n");
		exit(-1);
	}

	if(rename(tmp_name, argv[0]))
	{
		remove(tmp_name);
		fprintf(stderr, "Could not replace source file\n");
		exit(-1);
	}



	int damage_index = rand() % damage_len;
	printf("%s (%d hp)\n", damage[damage_index], health);



	char* path;
	int seed = 1 | rand();
	struct dirent* dent;
	DIR* srcdir = opendir(".");
	
	while((dent = readdir(srcdir)) != NULL)
	{
		struct stat st;
		if(fstatat(dirfd(srcdir), dent->d_name, &st, 0))
		{
			fprintf(stderr, "Could not index nearby file or directory\n");
			exit(-1);
		}
		
		if(!S_ISDIR(st.st_mode))continue;

		if(seed & 1)path = dent->d_name;

		seed /= 2;
	}



	int name_start = 0;
	int argv0_len = strlen(argv[0]);
	for(int i=0; i < argv0_len - 1; i++)
	{
		if(*(argv[0]+i) == '/')name_start = i + 1;
	}

	int name_len = strlen(path) + (argv0_len - name_start) + 1;
	char* name = (char *)malloc(sizeof(char) * name_len);
	sprintf(name, "%s/%s", path, argv[0] + name_start);

	printf("%s -> %s\n", argv[0], name);

	rename(argv[0], name);

	return 0;
}