#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdlib.h>
#include "buddy_allocator.h"
#include "pseudo_malloc.h"
#include <bits/mman-linux.h>

//due to some problems with mmap and linux...
#define MAP_ANONYMOUS 0x20

BuddyAllocator alloc;
BitMap bitmap;
char memory[BUDDY_ALLOCATOR_SIZE]; //the memory pool
uint8_t bitmap_buffer[BITMAP_SIZE]; //buddy allocator buffer/buffer for the bitmap

//initializes pseudo allocator
void pseudo_malloc_init() {
  BuddyAllocator_init(&alloc, BUDDY_ALLOCATOR_LEVELS, bitmap_buffer, BITMAP_SIZE, memory, BUDDY_ALLOCATOR_SIZE, MIN_BUDDY_SIZE);
}

//allocates memory of specified size size
void* pseudo_malloc(size_t size){
    //we basically have 3 possible cases:
    //1) size<=0
    if(size<=0){
      printf("cannot allocate if size is < or = 0...retry again :/\n");
      return NULL;
    }
    size += sizeof(int); //strategic move 
    //2) size<=page size / 4
    if(size<=(BUDDY_ALLOCATOR_THRESHOLD)){
      printf("allocating memory with homemade buddy allocator :P\n");
      return BuddyAllocator_malloc(&alloc, size);
    }
    //3) size>page size / 4
    else if(size>(BUDDY_ALLOCATOR_THRESHOLD)){
      printf("allocating memory with mmap syscall >_<\n");
      void * mmap_ptr= mmap(0, size, PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); //having some problems with MAP_ANONYMOUS flag in linux...
      if (mmap_ptr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
      printf("memory allocated correctly\n\n");
      return mmap_ptr;
    }
    return NULL;
}

void pseudo_free(void* mem_ptr){
    if(mem_ptr==NULL){
      printf("cannot deallocate because memory pointer is NULL...retry again :/\n");
      return;
    }
    if (mem_ptr >= (void *)memory && mem_ptr < (void *)memory + BUDDY_ALLOCATOR_SIZE * sizeof(char))
    {
      printf("deallocating memory allocated with homemade buddy allocator :)\n");
      BuddyAllocator_free(&alloc, mem_ptr);
    }
    else
    {
      printf("deallocating memory allocated with mmap syscall ;)\n");
      int m = munmap(mem_ptr, *((int *)mem_ptr));
      if(m==-1) perror("munmap");
      printf("memory deallocated correctly\n\n");
    }
}