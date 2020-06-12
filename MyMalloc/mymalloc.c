#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

#define META_SIZE sizeof(struct meta_block)



typedef struct meta_block *meta_block;


void *head_base = NULL;

struct meta_block
{
	size_t		size;
	meta_block	next;
	int 		isFree;

};

meta_block extend_space(meta_block last, size_t size);
meta_block find_free(meta_block *last, size_t size);

void * mymalloc(size_t size){
	meta_block ptr;
	
	if(!head_base) {
		ptr = extend_space(NULL, size);
		if(!ptr) return NULL;
		head_base = ptr;
	}
	else{
		meta_block last = head_base;
		ptr = find_free(&last, size);
		if(!ptr){
			ptr = extend_space(last, size);
		}	
	}
	
	return ptr;
}

meta_block find_free(meta_block *last, size_t size){
	meta_block current = head_base;
	while(current && !(current->isFree && current->size>=size)){
		*last = current;
		current = current->next;
	}
	return current;
}

meta_block extend_space(meta_block last, size_t size){
	meta_block ptr;
	ptr = sbrk(0);
	if(sbrk(META_SIZE + size)==(void *)-1) return NULL;
	ptr->size = size;
	ptr->next = NULL;
	if(last){
		last->next = ptr;
	}
	
	ptr->isFree = 0;
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