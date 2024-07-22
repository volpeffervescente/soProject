#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buddy_allocator.h"
#include "pseudo_malloc.h"


int main(int argc, char** argv) {

  //BuddyAllocator alloc;
  
  printf("_____________________________\n");
  printf(" testing Marty pseudo malloc\n");
  printf("_____________________________\n");
  printf("\n");
  // we initialize the buddy allocator
  /*printf("initializing the buddy allocator...\n");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BITMAP_SIZE,
                      memory,
                      MEMORY_SIZE,
                      MIN_BUCKET_SIZE);
  printf("\n");
  //still a lot of testing to be implemented...

  void* p1=BuddyAllocator_malloc(&alloc, 100);
  void* p2=BuddyAllocator_malloc(&alloc, 100);
  void* p3=BuddyAllocator_malloc(&alloc, 100000);
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_free(&alloc, p3);

  printf("buddy allocator testing went good!!!\n");*/
  
  printf("\t\n...\n");

  printf("initializing malloc/free...\n\n");

  pseudo_malloc_init();

  //correct memory allocation
  printf("\n");
  printf("memory allocation..\n");
  printf("\n");
  void* mem_ptr1 = pseudo_malloc(12);
  void* mem_ptr2 = pseudo_malloc(20);
  void* mem_ptr3 = pseudo_malloc(100);
  void* mem_ptr4 = pseudo_malloc(20000);
  void* mem_ptr5 = pseudo_malloc(400);
  void* mem_ptr6 = pseudo_malloc(1000);
  void* mem_ptr7 = pseudo_malloc(10000);
  void* mem_ptr8 = pseudo_malloc(100000);

  //freeing memory
  printf("\n");
  printf("freeing memory..\n");
  printf("\n");
  pseudo_free(mem_ptr1);
  pseudo_free(mem_ptr2);
  pseudo_free(mem_ptr3);
  pseudo_free(mem_ptr4);
  pseudo_free(mem_ptr5);
  pseudo_free(mem_ptr6);
  pseudo_free(mem_ptr7);
  pseudo_free(mem_ptr8);

  //wrong/invalid size/memory address
  printf("malloc...passing wrong sizes\n");
  void* mem_ptr9 = NULL;
  void* mem_ptr10 = pseudo_malloc(0);
  void* mem_ptr11 = pseudo_malloc(-14);

  pseudo_free(mem_ptr9);
  pseudo_free(mem_ptr10);
  pseudo_free(mem_ptr11);


  printf("pseudo malloc/free testing went good!!!\n");

  return 0;
  
}