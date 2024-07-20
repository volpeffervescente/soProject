#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buddy_allocator.h"
#include "pseudo_malloc.h"

//test inspired from buddy allocator test given in buddy allocator source code

#define BITMAP_SIZE 102400
#define BUDDY_LEVELS 9
#define MEMORY_SIZE (1024*1024)
#define MIN_BUCKET_SIZE (MEMORY_SIZE>>(BUDDY_LEVELS))

char buffer[BITMAP_SIZE];
char memory[MEMORY_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {

  printf("testing Marty pseudo malloc. . .\n");

  //2 we initialize the allocator
  printf("initializing the buddy allocator... ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BITMAP_SIZE,
                      memory,
                      MEMORY_SIZE,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");

  //still a lot of testing to be implemented...

  void* p1=BuddyAllocator_malloc(&alloc, 100);
  void* p2=BuddyAllocator_malloc(&alloc, 100);
  void* p3=BuddyAllocator_malloc(&alloc, 100000);
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_free(&alloc, p3);
  
}