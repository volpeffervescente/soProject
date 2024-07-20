#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"
#include <cassert>

//returns level according to the index
int levelIdx(size_t idx){
  return (int)floor(log2(idx));
};
//returns buddy index
int buddyIdx(int idx){
  if (idx&0x1){
    return idx-1;
  }
  return idx+1;
}
//returns parent index
int parentIdx(int idx){
  return idx/2;
}
//returns initial/start index
int startIdx(int idx){
  return (idx-(1<<levelIdx(idx)));
}
//returns right child index
int rightIdx(int idx){
  return ((idx*2)+2);
}
//returns left child index
int leftIdx(int idx){
  return ((idx*2)+1);
}


void BuddyAllocator_init(BuddyAllocator* alloc, int num_levels, char* bitmap_buffer, int bitmap_buffer_size, char* memory, int mem_size, int min_bucket_size){

  assert (num_levels<MAX_LEVELS);
  assert (bitmap_buffer_size>0);
  assert (min_bucket_size == buffer_size>>num_levels)

  // we need room also for level 0
  alloc->num_levels=num_levels;
  alloc->memory=memory;
  alloc->min_bucket_size=min_bucket_size;

  // we need enough memory to handle internal structures
  int num_bits= (int)(2*(pow(2, num_levels)))-1;
  assert(bitmap_buffer_size>=BitMap_getBytes(num_bits));
  BitMap_init(&(alloc->bit_map), num_bits, bitmap_buffer);

  
  //setting bit status for each node/bit

  //the root node, at the beginning, is the only one available
  BitMap_setBit(&(alloc->bit_map), 0, 1);
  for(int i=1; i<num_bits; i++){
    BitMap_setBit(&(alloc->bit_map), i, 0);
  }

  printf("bitmap BUDDY INITIALIZED\n");
};

int find_first_available_buddy(BuddyAllocator* alloc, int level){
  if (level<0)
    return 0;
  assert(level < alloc->num_levels);
  int firstIndex = (1<<level)-1;
  int lastIndex = ((1<<(level+1))-1);
  for(int i=firstIndex; i<=lastIndex; i++){
    if(BitMap_bit(&alloc->bit_map, i)==1)
      return i;
  }
  return -1;
}

void* BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level){
  if (level<0)
    return 0;
  assert(level <= alloc->num_levels);
  
  int buddy_idx=find_first_available_buddy(&alloc, level);
  if(buddy_idx!=-1){
    buddy_idx=leftIdx(buddy_idx);
    int other_buddy= rightIdx(buddy_idx);
  }
  else return -1;
  
  //the just picked buddy is now to be set to unavailable:
  BitMap_setBit(&alloc->bit_map, buddy_idx, 0);
  return buddy_idx;
}

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int idx){
  assert (idx>=0);
  int buddy_idx = buddyIdx(idx);
  int parent_idx = parentIdx(idx);
  
  //join
  //1. we destroy the two buddies in the free list;
  printf("merge \n");
  if(BitMap_bit(&alloc->bit_map, buddy_idx)){ //==1
    BitMap_setBit(&alloc->bit_map, idx);
    BitMap_setBit(&alloc->bit_map, buddy_idx);
    BitMap_setBit(&alloc->bit_map, parent_idx);
    //2. we release the parent
    BuddyAllocator_releaseBuddy(alloc, parent_idx);
  }
  //2. we release the parent
  BuddyAllocator_releaseBuddy(alloc, parent_idx);

}

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  // we determine the level of the page

  int level = alloc->num_levels -1;
  int b_size = alloc->min_bucket_size;
  while(b_size < size){
    b_size <<=1;
    level--;
  }
  printf("requested: %d bytes, level %d \n",
         size, level);

  // we get a buddy of that size;
  int idx = BuddyAllocator_getBuddy(alloc, level);
  if (idx==-1){
    printf("no memory available\n");
    return 0;
  }

  // we write in the memory region managed the buddy address
  int level_from_idx = levelIdx(idx);//level from idx
  int offset = (idx-((1<<level_from_idx))-1);//offset
  int block_dimension = (1<<(alloc->num_levels - level - 1))*alloc->min_bucket_size;//block dim
  char* mem_address = (int*)(alloc->memory + (offset*block_dimension));
  *((int*)mem_address)=idx;
  return (mem_address + sizeof(int));
}
//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem_addr) {
  // we retrieve the buddy from the system
  mem_addr -= sizeof(int); //we need to calculate back the real mem address
  BuddyListItem** buddy_ptr=(BuddyListItem**)p;
  BuddyListItem* buddy=*buddy_ptr;
  //printf("level %d", buddy->level);
  // sanity check;
  assert(buddy->start==p);
  BuddyAllocator_releaseBuddy(alloc, buddy);
  return;
}
