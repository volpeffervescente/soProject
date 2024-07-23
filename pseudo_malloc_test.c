#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bit_map.h"
#include "buddy_allocator.h"
#include "pseudo_malloc.h"

/*for small requests (< 1/4 of the page size) it uses a
buddy allocator. Clearly, such a buddy allocator can
manage at most page-size bytes For simplicity use a 
single buddy allocator, implemented with a bitmap 
that manages 1 MB of memory for these small allocations.

for large request (>=1/4 of the page size) uses a mmap.*/

int main(int argc, char** argv) {
  
  printf("_____________________________\n");
  printf(" testing Marty pseudo malloc\n");
  printf("_____________________________\n");
  printf("\n");

  printf("initializing malloc/free...\n\n");

  pseudo_malloc_init();

  printf("___initialization done___");

  //correct memory allocation
  printf("\n");
  printf("memory allocation-deallocation: various examples...\n\n");
  printf("\n");
  printf("______allocating mem_ptr1______\n");
  void* mem_ptr1 = pseudo_malloc(12);
  printf("______freeing mem_ptr1______\n");
  pseudo_free(mem_ptr1);

  printf("______allocating mem_ptr2______\n");
  void* mem_ptr2 = pseudo_malloc(20);
  printf("______freeing mem_ptr2______\n");
  pseudo_free(mem_ptr2);

  printf("______allocating mem_ptr3______\n");
  void* mem_ptr3 = pseudo_malloc(100);
  printf("______freeing mem_ptr3______\n");
  pseudo_free(mem_ptr3);

  printf("______allocating mem_ptr4______\n");
  void* mem_ptr4 = pseudo_malloc(20000);
  printf("______freeing mem_ptr4______\n");
  pseudo_free(mem_ptr4);

  printf("______allocating mem_ptr5______\n");
  void* mem_ptr5 = pseudo_malloc(400);
  printf("______freeing mem_ptr5______\n");
  pseudo_free(mem_ptr5);

  printf("______allocating mem_ptr6______\n");
  void* mem_ptr6 = pseudo_malloc(1000);
  printf("______freeing mem_ptr6______\n");
  pseudo_free(mem_ptr6);

  printf("______allocating mem_ptr7______\n");
  void* mem_ptr7 = pseudo_malloc(10000);
  printf("______freeing mem_ptr7______\n");
  pseudo_free(mem_ptr7);

  printf("______allocating mem_ptr8______\n");
  void* mem_ptr8 = pseudo_malloc(100000);
  printf("______freeing mem_ptr8______\n");
  pseudo_free(mem_ptr8);

  //wrong/invalid size/memory address
  printf("malloc...passing wrong sizes\n");
  printf("______allocating mem_ptr9______\n");
  void* mem_ptr9 = NULL;
  printf("______freeing mem_ptr9______\n");
  pseudo_free(mem_ptr9);

  printf("______allocating mem_ptr10______\n");
  void* mem_ptr10 = pseudo_malloc(0);
  printf("______freeing mem_ptr10______\n");
  pseudo_free(mem_ptr10);

  printf("______allocating mem_ptr11______\n");
  void* mem_ptr11 = pseudo_malloc(-14);
  printf("______freeing mem_ptr11______\n");
  pseudo_free(mem_ptr11);


  //if user wants to provide a memory size to allocate...
  if (argc != 2) {
        return 1;
    }
  int num_size = atoi(argv[1]);
  size_t size = (size_t)num_size;
  printf("\nuser choice. Memory to be allocated: size is %ld\n", size);
  printf("______allocating mem_ptr_user______\n");
  void* mem_ptr_user = pseudo_malloc(size);
  printf("______freeing mem_ptr_user______\n");
  pseudo_free(mem_ptr_user);


  printf("pseudo malloc/free testing went good!!!\n");

  return 0;
  
}