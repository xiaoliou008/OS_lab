#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>

#define BUF_SIZE 512

void intHandle(int sig);	//  SIGINT
void usrHandle(int sig);	//  SIGUSR1

pid_t childpid1, childpid2;
int pipefd[2];	

int main()
{
	// create pipe
	if(pipe(pipefd) == -1){
		perror("Can't open pipe\n");
		exit(EXIT_FAILURE);
	}
	
	//set signal
	signal(SIGINT, intHandle);

	childpid1 = fork();
	if(childpid1 == 0){	// child 1
		childpid1 = getpid();
		// set signal
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR1, usrHandle);

		// send message
		int cnt;
		FILE *out = fdopen(pipefd[1], "w");
		for(cnt = 1;cnt > 0;cnt++){
			fprintf(out, "I send you %d times.\n", cnt);
			fflush(out);
			sleep(1);		// 1 second
		}
	}
	
	childpid2 = fork();
	if(childpid2 == 0){	// child 2
		childpid2 = getpid();
		// seg signal
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR1, usrHandle);

		// receive message
		for(;;){
			char buf[BUF_SIZE];
			memset(buf, 0, sizeof(buf));
			read(pipefd[0], buf, BUF_SIZE); 
			printf("%s", buf);
		}
	
	}

	// parent
	for(;;){}
	
	return 0;
}

void intHandle(int sig)
{
	kill(childpid1, SIGUSR1);
	kill(childpid2, SIGUSR1);	// debug SIGUSR2
	wait(NULL);	
	wait(NULL);
	if(close(pipefd[0]) == -1){
		perror("Can't close pipe\n");
		exit(EXIT_FAILURE);
	}
	if(close(pipefd[1]) == -1){
		perror("Can't close pipe\n");
		exit(EXIT_FAILURE);
	}
	printf("Parent Process is Killed!\n");
	exit(EXIT_SUCCESS);
}

void usrHandle(int sig)
{
	if(close(pipefd[0]) == -1){
		perror("Can't close pipe\n");
		exit(EXIT_FAILURE);
	}
	if(close(pipefd[1]) == -1){
		perror("Can't close pipe\n");
		exit(EXIT_FAILURE);
	}
	printf("Child Process %d is Killed by Parent!\n", ((getpid()==childpid1) ? 1 : 2));
	exit(EXIT_SUCCESS);
}






/*
			for(int i=0;i<BUF_SIZE;i++){
				if(buf[i] == '\n'){
					putchar(buf[i]);
					break;
				}
				else if(buf[i] == '\0'){
					break;
				}
				else putchar(buf[i]);
			}
*/
//			printf("%x\n", buf[0]);
//			printf("%d", buf[20]);
