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


//becero ma mi sono rotta
#define MAP_ANONYMOUS 0x20


/*for small requests (< 1/4 of the page size) it uses a
buddy allocator. Clearly, such a buddy allocator can
manage at most page-size bytes For simplicity use a 
single buddy allocator, implemented with a bitmap 
that manages 1 MB of memory for these small allocations.

for large request (>=1/4 of the page size) uses a mmap.*/

BuddyAllocator alloc;
BitMap bitmap;
char memory[BUDDY_ALLOCATOR_SIZE];
uint8_t bitmap_buffer[BITMAP_SIZE]; //buddy allocator buffer


void pseudo_malloc_init() {
  BuddyAllocator_init(&alloc, BUDDY_ALLOCATOR_LEVELS, bitmap_buffer, BITMAP_SIZE, memory, BUDDY_ALLOCATOR_SIZE, MIN_BUDDY_SIZE);
}

void* pseudo_malloc(size_t size){
    //abbiamo principalmente 3 casi possibili:
    //1) size<=0
    if(size<=0){
      printf("cannot allocate if size is < 0...retry again :/\n");
      return NULL;
    }
    size += sizeof(int); //strategic move 
    //2) size<=page size / 4
    if(size<=(BUDDY_ALLOCATOR_THRESHOLD)){
      printf("allocating memory with homemade buddy allocator :P, size is %ld.\n", size);
      return BuddyAllocator_malloc(&alloc, size);
    }
    //3) size>page size / 4
    else if(size>(BUDDY_ALLOCATOR_THRESHOLD)){
      printf("allocating memory with mmap syscall >_<\n");
      void * mmap_ptr= mmap(0, size, PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); //having some problems with MAP_ANONYMOUS flag in linux...
      if (mmap_ptr == MAP_FAILED) {
        perror("mmap");
        assert("mmap allocation not gone well... :(" && mmap_ptr != MAP_FAILED);
        return NULL;
    }
      printf("memory allocated correctly\n\n");
      return mmap_ptr;
    }
    return NULL;
}

void pseudo_free(void* mem_ptr){
  /*
    //abbiamo principalmente 3 casi possibili:
    //1) mem_ptr is NULL
    if(mem_ptr==NULL){
      printf("cannot deallocate because memory pointer is NULL...retry again :/\n");
      return;
    }
    size_t size = *((size_t*)mem_ptr); 
    size -= sizeof(int);//strategic move 

    //2) size<=page size / 4
    if(size<=(BUDDY_ALLOCATOR_THRESHOLD)){
      printf("deallocating memory allocated with homemade buddy allocator :)\n");
      BuddyAllocator_free(&alloc, mem_ptr);
      return;
    }
    //3) size>page size / 4
    else if(size>(BUDDY_ALLOCATOR_THRESHOLD)){
      printf("deallocating memory allocated with mmap syscall ;)\n");
      int mu = munmap(mem_ptr, size);
      assert("mmap deallocation not gone well... :(" && mu!=-1);
      printf("memory deallocated correctly\n\n");
      return;
    }*/
    //had to find another way to free memory, since I've made a mess previously. 
    if (mem_ptr >= (void *)memory && mem_ptr < (void *)memory + BUDDY_ALLOCATOR_SIZE * sizeof(char))
    {
      printf("deallocating memory allocated with homemade buddy allocator :)\n");
      BuddyAllocator_free(&alloc, mem_ptr);
    }
    else
    {
      printf("deallocating memory allocated with mmap syscall ;)\n");
      munmap(mem_ptr, *((int *)mem_ptr));
    }
}