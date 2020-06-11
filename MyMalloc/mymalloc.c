#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

void * mymalloc(size_t size){
	void *ptr;
	ptr = sbrk(0);
	if (sbrk(size)==(void*)-1){
		return NULL;
	}
	return ptr;
}




int main(){
	char *p;
	p = (char *)mymalloc(50);
	if(p==NULL){
		fprintf(stderr, "Error in mymalloc\n" );
	}
	strcpy(p, "Hi, I am Dheeraj, This is a test");
	printf("And the output is: %s\n", p);
}