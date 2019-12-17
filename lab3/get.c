#include<unistd.h>							// otherwise read implicit declaration
#include"svshm.h"

// id of the semaphore set
int semid;

// shared memory id
int shmid;

// pointer of the buf
struct shmseg *shmp;

int main(int argc, char *argv[])
{
	// get the semaphore created by parent
	semid = get_Sem(SEM_KEY, 2);
	if(semid < 0) exit(EXIT_FAILURE);

	// get the shared memory created by parent
	shmid = shmget(SHM_KEY, 0, 0);
	if(shmid == -1){
		perror("child get shmget error");
		exit(EXIT_FAILURE);
	}
	// attach the shared memory
	shmp = (struct shmseg*)shmat(shmid, NULL, 0);
	struct shmseg *start = shmp;
	if(shmp == (void *)-1){
		perror("child get shmat error");
		exit(EXIT_FAILURE);
	}

	// open input file 
	int fd = open("input.txt", O_RDONLY);

	// get data from input.txt to shared memory
	for(int i=0;;i = (i + 1) % BUF_NUM){
		shmp = start + i;
		P(semid, WRITE_SEM);				

		shmp->cnt = read(fd, shmp->buf, BUF_SIZE);
		if(shmp->cnt == -1){
			perror("read error");
			exit(EXIT_FAILURE);
		}
		printf("write data to the %ith buf\n", i);

		V(semid, READ_SEM);

		if(shmp->cnt == 0)					// read EOF
			break;
	}

	if(close(fd) == -1){
		perror("close input.txt error");
		exit(EXIT_FAILURE);
	}

	// detach the shared memory
	if(shmdt(start) == -1){					// BUG user pointer not shmid
		perror("child get shmdt error");
		exit(EXIT_FAILURE);
	}
	
	printf("Child_get finish\n");	
	return EXIT_SUCCESS;
}
