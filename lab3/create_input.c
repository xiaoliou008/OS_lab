#include<stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp = fopen("input.txt", "w");
	for(int i=0;i<100000;i++)
		fprintf(fp, "i = %d\n", i);
	fclose(fp);
	return 0;	
}
