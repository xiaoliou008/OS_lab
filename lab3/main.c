#include<unistd.h>
#include<sys/wait.h>
#include"svshm.h"

// two child id
pid_t write_pid, read_pid;

// id of the semaphore set
int semid;

// shared memory id
int shmid;

int main(int argc, char *argv[])
{
	// create shared memory
	shmid = shmget(SHM_KEY, BUF_NUM * sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
	if(shmid == -1){
		perror("semget create error");
		return EXIT_FAILURE;
	}

	// create two semaphore and get the id
	semid = create_Sem(SEM_KEY, 2);
	if(semid < 0){
		exit(EXIT_FAILURE);
	}
	// initial the semaphore
	set_N(semid, WRITE_SEM, BUF_NUM);		// be able to write into the shared buf
	set_N(semid, READ_SEM, 0);				// be able to read from the shared buf

	
	// create child process
	switch(write_pid = fork()){
	case -1:
		perror("fork child1 get error");
		exit(EXIT_FAILURE);

	case 0:									// child1 get process
		printf("Child1 get created\n");
		execl("./get", "get", NULL);
		exit(EXIT_SUCCESS);

	default:								// parent proccess
		break;
	}
	
	switch(read_pid = fork()){
	case -1:
		perror("fork child2 put error");
		exit(EXIT_FAILURE);

	case 0:									// child2 put process
		printf("Child2 put created\n");
		execl("./put", "put", NULL);
		exit(EXIT_SUCCESS);

	default:								// parent process
		break;
	}

	// wait child
	waitpid(write_pid, NULL, 0);
	waitpid(read_pid, NULL, 0);

	// destroy the semaphore
	if(destroy_Sem(semid) < 0){
		exit(EXIT_FAILURE);
	}

	// free the shared memory
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		perror("remove shmid error");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}







/*
	// attach the shared memory and create buf link
	struct shmseg* start = (struct shmseg*)shmat(shmid, NULL, SHM_R | SHM_W);
	// needless
	struct shmseg* p = start;
	for(int i = 0; i < BUF_NUM-1; i++){
		p->next = p + 1;
		p++;
	}
	p->next = start;
*/
	
/*
	// debug
	p = start;
	int fin = open("input.txt", O_RDONLY);
	int fout = open("output.txt", O_WRONLY | O_CREAT);
	for(int i=0;i<BUF_NUM;i++){		// BUF_NUM = 8
//		printf("i = %d\n", i);
//		printf("sizeof = %lu\nBUF_SIZE = %d\n", sizeof(p->buf), BUF_SIZE);
//		read(fin, p->buf, sizeof(p->buf));		// BUF_SIZE = 1024
//		write(fout, p->buf, sizeof(p->buf));
		read(fin, p->buf, BUF_SIZE);
		write(fout, p->buf, BUF_SIZE);
		p = p->next;
	}
	return 0;
*/

/*
	// detach the shared memory
	if(shmdt(start) == -1){
		perror("shmdt error");
		exit(EXIT_FAILURE);
	}
	start = p = NULL;
*/
