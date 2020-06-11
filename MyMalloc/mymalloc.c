#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

#define META_SIZE sizeof(struct meta_block)

typedef struct meta_block *meta_block;

struct meta_block
{
	size_t		size;
	meta_block	next;
	int 		isFree;

};

void * mymalloc(size_t size){
	meta_block ptr;
	ptr = sbrk(0);
	if (sbrk(META_SIZE+size)==(void*)-1){
		return NULL;
	}
	ptr->size = size;
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