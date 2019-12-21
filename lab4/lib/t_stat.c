#include"t_stat.h"

void displayStat(const struct stat *sb)
{
	switch(sb->st_mode & S_IFMT){
	case S_IFREG:	printf("-");	break;
	case S_IFDIR:	printf("d");	break;
	case S_IFCHR:	printf("c");	break;
	case S_IFBLK:	printf("b");	break;
	case S_IFLNK:	printf("l");	break;
	case S_IFIFO:	printf("p");	break;
	case S_IFSOCK:	printf("s");	break;
	default:		printf("unknown file type\n");
	}
	printf("%s", filePermStr(sb->st_mode));
	printf(" %ld", (long) sb->st_nlink);
	printf(" %s %s", getUserName(sb->st_uid), getGroupName(sb->st_gid));
	printf(" %lld", (long long) sb->st_size);
	strcpy(timestr, ctime(&sb->st_mtime));
	timestr[strlen(timestr) - 1] = '\0';
	printf(" %s", timestr);
	return;
}

char * getUserName(uid_t uid)
{
	struct passwd *pwd;
	pwd = getpwuid(uid);
	return (pwd == NULL) ? NULL : pwd->pw_name;
}

char * getGroupName(gid_t gid)
{
	struct group *grp;
	grp = getgrgid(gid);
	return (grp == NULL) ? NULL : grp->gr_name;
}	

char * filePermStr(mode_t perm)
{
	static char str[STR_SIZE];
	snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
		(perm & S_IRUSR) ? 'r' : '-',
		(perm & S_IWUSR) ? 'w' : '-',
		(perm & S_IXUSR) ? 'x' : '-',
		(perm & S_IRGRP) ? 'r' : '-',
		(perm & S_IWGRP) ? 'w' : '-',
		(perm & S_IXGRP) ? 'x' : '-',
		(perm & S_IROTH) ? 'r' : '-',
		(perm & S_IWOTH) ? 'w' : '-',
		(perm & S_IXOTH) ? 'x' : '-');
	return str;
}

char * filePermStrSpecial(mode_t perm)
{
	static char str[STR_SIZE];
	snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
		(perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
		(perm & S_IXUSR) ?
			((perm & S_ISUID) ? 's' : 'x') :
			((perm & S_ISUID) ? 'S' : '-'),
		(perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
		(perm & S_IXGRP) ?
			((perm & S_ISGID) ? 's' : 'x') :
			((perm & S_ISGID) ? 'S' : '-'),
		(perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
		(perm & S_IXOTH) ?
			((perm & S_ISVTX) ? 't' : 'x') :
			((perm & S_ISVTX) ? 'T' : '-'));
	return str;
}
