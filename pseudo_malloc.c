#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdlib.h>
#include "buddy_allocator.h"
#include "pseudo_malloc.h"
#include <bits/mman-linux.h>

/*for small requests (< 1/4 of the page size) it uses a
buddy allocator. Clearly, such a buddy allocator can
manage at most page-size bytes For simplicity use a 
single buddy allocator, implemented with a bitmap 
that manages 1 MB of memory for these small allocations.

for large request (>=1/4 of the page size) uses a mmap.*/

BuddyAllocator alloc;
BitMap bitmap;
int bitmap_buffer_size [(1<<BUDDY_ALLOCATOR_LEVELS)-1]; //also buddyes num
char* memory [BUDDY_ALLOCATOR_SIZE];

void pseudo_malloc_init(){
    uint8_t* bitmap_buffer = (uint8_t*)bitmap_buffer_size + (7 * sizeof(int));
 
    BitMap_init(&bitmap, bitmap_buffer_size, &bitmap_buffer);

    BuddyAllocator_init(&alloc, BUDDY_ALLOCATOR_LEVELS,
                      bitmap_buffer, bitmap_buffer_size, memory,
                       BUDDY_ALLOCATOR_SIZE, 1);
}

void* pseudo_malloc(size_t size){
    //abbiamo principalmente 3 casi possibili:
    //1) zize<=0
    if(size<=0){
      printf("cannot allocate if size is < 0...retry again :/\n");
      return 0;
    }
    size+=sizeof(int); //strategic move 
    //2) size<=page size / 4
    if(size<=(PAGE_SIZE/4)){
      printf("allocating memory with homemade buddy allocator :P\n");
      return BuddyAllocator_malloc(&alloc, size);
    }
    //3) size>page size / 4
    else if(size<=(PAGE_SIZE/4)){
      printf("allocating memory with mmap syscall >_<\n");
      void * mmap_ptr= mmap(0, size, PROT_READ | PROT_WRITE , MAP_PRIVATE /*| MAP_ANONYMOUS*/, -1, 0); //having some problems with MAP_ANONYMOUS flag in linux...
      assert("mmap allocation not gone well... :(" && mmap_ptr!=MAP_FAILED);
      return mmap_ptr;
    }
}

void pseudo_free(void* mem_ptr){

}