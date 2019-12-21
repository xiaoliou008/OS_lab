#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include"t_stat.h"

void printdir(char *dir, int depth)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(dir)) == NULL){
		printf("Open %s error\n", dir);
		return;
	}
	if(chdir(dir) == -1){
		perror("chdir error");
		return;
	}
	entry = readdir(dp);
	while(entry != NULL){
		if(lstat(entry->d_name, &statbuf) == -1){
			printf("Get stat %s error\n", entry->d_name);
			continue;
		}
		if((statbuf.st_mode & S_IFMT) == S_IFDIR){		// directory
			if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){	// skip . or ..
				entry = readdir(dp);
				continue;			
			}	
			printf("Depth: %d ", depth);
			displayStat(&statbuf);
			printf(" %s\n", entry->d_name);
			printdir(entry->d_name, depth + 4);
		}
		else{
			printf("Depth: %d ", depth);
			displayStat(&statbuf);
			printf(" %s\n", entry->d_name);
		}
		entry = readdir(dp);
	}
	if(chdir("..") == -1){
		perror("chdir back error");
		return;
	}
	closedir(dp);
	return;
}

int main(int argc, char *argv[])
{
	char *path = ".";
	if(argc > 1) path = argv[1];
	printdir(path, 0);
	return EXIT_SUCCESS;
}
