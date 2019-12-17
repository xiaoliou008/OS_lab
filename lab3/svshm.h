#include<sys/stat.h>
#include<fcntl.h>		// O_RDONLY O_WRONLY
#include<sys/shm.h>
#include"sem.h"

#define SHM_KEY		0x1234
#define SEM_KEY		0x5678

#define OBJ_PERMS	0666

#define WRITE_SEM	0
#define READ_SEM	1
#define BUF_NUM		8

#ifndef BUF_SIZE
#define BUF_SIZE	1024
#endif

struct shmseg{
	int cnt;				// the size of data in the buf
	char buf[BUF_SIZE];		// store the data
	//struct shmseg *next;
};
