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


void rewrite(const char* file_name, int health)
{
	// Generate tmp name for code modification
	int tmp_name_len = strlen(file_name) + 12;
	char* tmp_name = (char *)malloc(sizeof(char) * tmp_name_len);
	sprintf(tmp_name, "%s.tmp%08X", file_name, rand());


	// Open program in read mode
	FILE *in_file = fopen(file_name, "r");
	if(in_file == NULL)
	{
		fprintf(stderr, "Could not open source file\n");
		exit(-1);
	}

	// Create tmp file in write mode
	FILE *out_file = fopen(tmp_name, "w");
	if(out_file == NULL)
	{
		fprintf(stderr, "Could not write to temp file\n");
		exit(-1);
	}


	// Copy program data and change health
	// (Note: This algorithm only works, if flag[0] != flag[1])
	int curr = 0;
	int prog = 0;

	while((curr = fgetc(in_file)) != EOF)
	{
		if(prog == flag_len)fputc((char)(health), out_file);
		else fputc(curr, out_file);

		if(curr == *(flag + prog))prog++;
		else if(curr == *flag)prog = 1;
		else prog = 0;
	}


	// Close both files
	fclose(in_file);
	fclose(out_file);


	// Make the modified file executable
	if(chmod(tmp_name, 0x1ed))
	{
		remove(tmp_name);
		fprintf(stderr, "Could not chmod temp file\n");
		exit(-1);
	}

	// Overwrite the old executable
	if(rename(tmp_name, file_name))
	{
		remove(tmp_name);
		fprintf(stderr, "Could not replace source file\n");
		exit(-1);
	}
}


char *random_subdir()
{
	int seed = rand();

	char* dir_name = NULL;
	struct dirent* dent;
	DIR* srcdir = opendir(".");	

	// Loop over all files in pwd
	while((dent = readdir(srcdir)) != NULL)
	{
		struct stat st;
		if(fstatat(dirfd(srcdir), dent->d_name, &st, 0))
		{
			fprintf(stderr, "Could not index nearby file or directory\n");
			exit(-1);
		}

		// Skip everything that isn't a directory
		if(!S_ISDIR(st.st_mode))continue;

		// Save directory
		if(seed & 1 || dir_name == NULL)
		{
			dir_name = dent->d_name;
		}

		if((seed /= 2) == 0)break;
	}

	return dir_name;
}


void teleport(const char* file_name)
{
	// Find a directory to teleport to
	char* mv_dir = random_subdir();

	// Isolate the name of the executable
	char *bin_name = NULL;
	int argv0_len = strlen(file_name);
	for(int i=0; i < argv0_len - 1; i++)
	{
		if(*(file_name+i) == '/')
		{
			bin_name = (char *)file_name + i + 1;
		}
	}

	// Move executable by renaming it
	int mv_name_len = strlen(mv_dir) + strlen(bin_name) + 1;
	char* mv_name = (char *)malloc(sizeof(char) * mv_name_len);
	sprintf(mv_name, "%s/%s", mv_dir, bin_name);
	printf("%s -> %s\n", file_name, mv_name);
	rename(file_name, mv_name);
}


int main(int argc, char* argv[])
{
	int health = *(flag + flag_len);
	
	srand(time(NULL) - health);

	// Catch error that should never happen
	// (it you remove this check, the compiler screams at you for never using argc)
	if(argc < 1)
	{
		fprintf(stderr, "Something went horribly wrong\n");
		exit(-1);
	}

	char* org_name = argv[0];


	// Scream in pain and die
	if(health < 1)
	{
		int death_index = rand() % death_len;
		printf("%s (0 hp)\n", death[death_index]);

		remove(org_name);
		exit(0);
	}


	// Update health
	rewrite(org_name, health - 1);


	// Scream in pain
	int damage_index = rand() % damage_len;
	printf("%s (%d hp)\n", damage[damage_index], health);


	// Move to random directory
	teleport(org_name);


	return 0;
}