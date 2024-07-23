#pragma once
#include "bit_map.h"

#define MAX_LEVELS 16

typedef struct  {
  BitMap bit_map;//bitmap pointer to track buddy status
  int num_levels;//number of levels in the buddy system
  char* memory; // the memory area to be managed
  int mem_size;//total memory size
  int min_bucket_size; // the minimum page of RAM that can be returned/that can be allocated
} BuddyAllocator;


// initializes the buddy allocator, and checks that the buffer is large enough
void BuddyAllocator_init(BuddyAllocator* alloc, int num_levels, uint8_t* bitmap_buffer, int bitmap_buffer_size, char* memory, int mem_size, int min_bucket_size);

// returns (allocates) a buddy at a given level.
// side effect on the internal structures
// 0 id no memory available
int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level);


// releases an allocated buddy, performing the necessary joins
// side effect on the internal structures
void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int idx);


//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, size_t size);

//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem_addr);
//helper function to calcolate indexes
int rightIdx(int idx);

int leftIdx(int idx);

int find_first_available_buddy(BuddyAllocator* alloc, int level);

int Buddyallocator_level(BuddyAllocator* alloc, size_t size);