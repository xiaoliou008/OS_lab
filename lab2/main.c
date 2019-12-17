#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include"sem.h"

#define SEM_PRINT 	0
#define	SEM_ADD		1

// id of the semaphore set
int semid;

// thread1 print the value of a
void *t1_print(void *arg);
// thread2 add the value of a
void *t2_add(void *arg);

int a = 0;		// global value shared by two thread

int main(int argc, char *argv[])
{
	// create two semaphore and get the id
	semid = create_Sem(IPC_PRIVATE, 2);
	if(semid < 0){
		exit(EXIT_FAILURE);
	}

	// initial the semaphore
	set_N(semid, SEM_PRINT, 0);		// be able to print the value of a
	set_N(semid, SEM_ADD, 1);		// be able to change the value of a

	// thread1 and thread 2
	pthread_t t1, t2;

	// create new thread 1 and 2
	int s = 0;
	s = pthread_create(&t1, NULL, t1_print, NULL);
	if(s != 0){
		perror("Create thread 1 error");
		exit(EXIT_FAILURE);
	}		
	s = pthread_create(&t2, NULL, t2_add, NULL);
	if(s != 0){
		perror("Create thread 2 error");
		exit(EXIT_FAILURE);
	}		

	// join the thread 1 and 2
	s = pthread_join(t1, NULL);
	if(s != 0){
		perror("Join thread 1 error");
		exit(EXIT_FAILURE);
	}		
	s = pthread_join(t2, NULL);
	if(s != 0){
		perror("Join thread 2 error");
		exit(EXIT_FAILURE);
	}		

	// destroy the semaphore
	s = destroy_Sem(semid);
	if(s < 0){
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

// thread1 start
void *t1_print(void *arg)
{
	for(int i = 1; i <= 100; i++){
		P(semid, SEM_PRINT);	// wait thread2 change the value of a
		printf("%d\n", a);		// print the value of the shared variable
		V(semid, SEM_ADD);		// let thread2 continue its work
	}
	return NULL;
}

// thread2 start
void *t2_add(void *arg)
{
	for(int i = 1; i <= 100; i++){
		P(semid, SEM_ADD);		// wait thread1 print the value of a
		a += i;					// change the value of a
		V(semid, SEM_PRINT);	// let thread1 continue its work
	}
	return NULL;
}
