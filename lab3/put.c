#include<unistd.h>
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
	shmid = shmget(SHM_KEY, 0, SHM_RDONLY);
	if(shmid == -1){
		perror("child put shmget error");
		exit(EXIT_FAILURE);
	}
	// attach the shared memory
	shmp = (struct shmseg*)shmat(shmid, NULL, 0);
	struct shmseg *start = shmp;
	if(shmp == (void *)-1){
		perror("child put shmat error");
		exit(EXIT_FAILURE);
	}

	// open output file(if not exist then create it)
	int fd = open("output.txt", O_WRONLY | O_CREAT);

	// get data from shared memory and write into output.txt
	for(int i=0;;i = (i + 1) % BUF_NUM){
		shmp = start + i;
		P(semid, READ_SEM);				

		if(shmp->cnt == 0)
			break;		

		if(write(fd, shmp->buf, shmp->cnt) != shmp->cnt){
			perror("write output error");
			exit(EXIT_FAILURE);
		}
		printf("read data from the %dth buf\n", i);

		V(semid, WRITE_SEM);
	}

	if(close(fd) == -1){
		perror("close output.txt error");
		exit(EXIT_FAILURE);
	}

	// detach the shared memory
	if(shmdt(start) == -1){
		perror("child put shmdt error");
		exit(EXIT_FAILURE);
	}
	
	printf("Child_put finish\n");
	return EXIT_SUCCESS;
}