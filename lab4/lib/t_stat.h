#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<ctype.h>
#include<string.h>

#define STR_SIZE 		sizeof("rwxrwxrwx")
#define TIME_STR_SIZE	80

char timestr[TIME_STR_SIZE];

void displayStat(const struct stat *sb);
char * filePermStr(mode_t perm);
char * filePermStrSpecial(mode_t perm);
char * getUserName(uid_t uid);
char * getGroupName(gid_t gid);
