#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

#define META_SIZE 20
#define align(x) (((((x)-1)>>2)<<2)+4)



typedef struct meta_block *meta_block;


void *head_base = NULL;

struct meta_block
{
	size_t		size;
	meta_block	next;
	int 		isFree;
	void		*ptr;
	meta_block	prev;
	char		data[1];

};

meta_block extend_space(meta_block last, size_t size);
meta_block find_free(meta_block *last, size_t size);



meta_block find_free(meta_block *last, size_t size){
	meta_block current = head_base;
	while(current && !(current->isFree && current->size>=size)){
		*last = current;
		current = current->next;
	}
	return current;
}

meta_block extend_space(meta_block last, size_t size){
	meta_block p;
	p= sbrk(0);
	if(sbrk(META_SIZE + size)==(void *)-1) return NULL;
	p->size = size;
	p->next = NULL;
	p->prev = last;
	if(last){
		last->next = p;
	}
	
	p->isFree = 0;
	return p;
}

meta_block get_block(void *ptr){
	char * tmp;
	tmp = ptr;
	ptr = tmp - META_SIZE;
	return ptr;
}

meta_block fusion(meta_block b){
	if(b->next && (b->next->isFree)){
		b->size += META_SIZE + b->next->size;
		b->next = b->next->next;
		if(b->next)
			b->next->prev = b;
	}
	return b;
}

void * mymalloc(size_t size){
	meta_block ptr;
	size_t s = align(size);
	if(!head_base) {
		ptr = extend_space(NULL, s);
		if(!ptr) return NULL;
		head_base = ptr;
	}
	else{
		meta_block last = head_base;
		ptr = find_free(&last, s);
		if(!ptr){
			ptr = extend_space(last, s);
		}
		else{
			ptr->isFree = 0;
		}	
	}
	
	return ptr->data;
}

void * mycalloc(size_t number, size_t size){
	size_t *new;
	new = mymalloc(number * size);
	if(new){
		size_t siz = align(number*size)<<2;
		for (size_t i = 0; i < siz; ++i)
		{
			new[i] =0;
		}
	}
	return new;
}

void free(void *ptr){
	if(!ptr){
		return;
	}
	meta_block b;
	b = get_block(ptr);
	
	b->isFree =1;
	

	/*if(b->prev && b->prev->isFree){
		printf("in free2.\n");
		b = fusion(b->prev);
	}
	printf("in free2.\n");
	if(b->next && b->next->isFree){

		b = fusion(b);
	}
	else{
		printf("free3.\n");
		if(b->prev)
			b->prev->next = NULL;
		else
			head_base = NULL;
		brk(b);
	}*/

}



int main(){
	int *p;
	p = (int *)mymalloc(25*sizeof(int));
	if(p==NULL){
		fprintf(stderr, "Error in mymalloc\n" );
	}
	for(int i = 0; i<25; i++){
		p[i] = i+1;
	}
	
	for(int i = 0; i<25; i++){
		printf("%d\n", p[i]);
	}
	printf("Before free.\n");
	free(p);
	printf("After free.\n");
	for(int i = 0; i<25; i++){
		printf("%d\n", p[i]);
	}

	//strcpy(p, "Hi, I am Dheeraj, This is a test");
	//printf("And the output is: %s\n", p);
}