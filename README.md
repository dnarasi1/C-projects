# Memory allocator

- Simple malloc like memory allocator implemented in C. 
- Implemented malloc, calloc and free functions. 
- The functions use brk(used to set the end of the data segment to the specified value) and sbrk system call (increments the data segment size by the specified amount).
- Also implemented a find free block function which looks for a free chunk in the linked list of blocks, if the free chunk is sufficient for the required size, we return the address or else we just call the extend_space function. 
