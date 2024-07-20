#pragma once
#include "bit_map.h"

#define MAX_LEVELS 16

typedef struct  {
  BitMap bit_map;//bitmap pointer
  int num_levels;
  char* memory; // the memory area to be managed
  int mem_size;
  int min_bucket_size; // the minimum page of RAM that can be returned
} BuddyAllocator;


// computes the size in bytes for the buffer of the allocator
int BuddyAllocator_calcSize(int num_levels);


// initializes the buddy allocator, and checks that the buffer is large enough
void BuddyAllocator_init(BuddyAllocator* alloc, int num_levels, char* bitmap_buffer, int bitmap_buffer_size, char* memory, int mem_size, int min_bucket_size);

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);
