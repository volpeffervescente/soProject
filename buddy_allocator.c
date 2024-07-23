#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "bit_map.h"
#include "buddy_allocator.h"

//returns level according to the index
int levelIdx(size_t idx){
  return (int)floor(log2(idx+1));
};
//returns buddy index
int buddyIdx(int idx){
  if (idx&0x1){ //if idx is odd
    return idx-1; 
  }
  else if(idx==0)
    return 0;
  else
    return idx+1;
}
//returns parent index
int parentIdx(int idx){
  return (idx-1)/2;
}
//returns initial/start index
int startIdx(int idx){
  return (idx-(1<<levelIdx(idx))-1);
}
//returns right child index
int rightIdx(int idx){
  return ((idx*2)+2);
}
//returns left child index
int leftIdx(int idx){
  return ((idx*2)+1);
}

//initializes the buddy allocator
void BuddyAllocator_init(BuddyAllocator* alloc, int num_levels, uint8_t* bitmap_buffer, int bitmap_buffer_size, char* memory, int mem_size, int min_bucket_size){
  assert (num_levels<MAX_LEVELS); //ensure num of levels is within the limit
  assert (bitmap_buffer_size>0);
  if (log2(mem_size) != floor(log2(mem_size))){  //floor arrotonda
  //se mem_size non è un intero potenza di 2, 
        mem_size = min_bucket_size << num_levels;
  }
  alloc->num_levels=num_levels;
  alloc->memory=memory;
  alloc->min_bucket_size=min_bucket_size;

  // calculating number of bit needed for the bitmap
  int num_bits= (int)(2*(pow(2, num_levels)))-1;
  assert(bitmap_buffer_size>=BitMap_getBytes(num_bits));
  BitMap_init(&(alloc->bit_map), num_bits, bitmap_buffer);

  //the root node, at the beginning, is the only one available
  BitMap_setBit(&(alloc->bit_map), 0, 1);
  printf("bitmap BUDDY INITIALIZED\n");
};
//finds the first available buddy at a given level
int find_first_available_buddy(BuddyAllocator* alloc, int level){
  if (level<0)
    return -1;
  assert(level < alloc->num_levels);
  int firstIndex = (1<<level)-1; //2alla n -1 (first index at this level)
  printf("firstIndex value is :   %d \n", firstIndex);
  int lastIndex = ((1<<(level+1))-2); // Index of the last block at this level
  printf("lastIndex value is :   %d  \n", lastIndex);
  //for(int i=firstIndex; i<=lastIndex; i++){
  int i = firstIndex;
  while(i<=lastIndex){
    if(BitMap_bit(&(alloc->bit_map), i)!=0){
      printf("---find_first_available_buddy--free buddy found! \n\n");
      return i;
    }
    i++;
  }
  printf("---find_first_available_buddy--free buddy NOT found! \n\n");
  return -1;
}

int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level){
  //printf("--getBuddy--level value at the beginning of the function is   %d\n", level);
  if (level<0)
    return -1;
  assert(level <= alloc->num_levels);
  int buddy_idx=find_first_available_buddy(alloc, level);
  printf("buddy_idx after find_first_available_buddy: %d\n", buddy_idx);
  if(buddy_idx==-1){
    int parent_idx = BuddyAllocator_getBuddy(alloc, level-1);
    printf("parent_idx: %d\n", parent_idx);
    if(parent_idx!=-1){
      buddy_idx=leftIdx(parent_idx);
      int other_buddy= rightIdx(parent_idx);
      //the other buddy is now to be set to available:
      BitMap_setBit(&alloc->bit_map, other_buddy, 1);
    }
    else if(parent_idx==-1){
      return -1;
    }
    //the just picked buddy is now to be set to unavailable:
    BitMap_setBit(&alloc->bit_map, buddy_idx, 0);
  }
  return buddy_idx;
}

//calculates the level based on the size
int Buddyallocator_level(BuddyAllocator* alloc, size_t size){
  int level = alloc->num_levels -1;
  size_t b_size = alloc->min_bucket_size;
  printf("Calculating level for size: %ld\n", size);
  while(level>=0){
    printf("At level %d with block size %ld\n", level, b_size);
    if(size<=b_size){
      printf("Found level: %d for size: %zu\n", level, size);
      return level;
    }
    b_size = b_size * 2; 
    level--; //salgo al livello superiore
  }
  printf("No valid level found for size: %zu\n", size);
  return -1;
}

//releases an allocated buddy and merges if possible
void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int idx){
  assert (idx>=0);
  int buddy_idx = buddyIdx(idx);
  int parent_idx = parentIdx(idx);
  //join
  printf("merging buddies!\n");
  //...probably another check on the budddy idx needed. . .
  if(buddy_idx==-1){
    BitMap_setBit(&alloc->bit_map, idx, 1);
  }
  //yes it was...
  if(BitMap_bit(&alloc->bit_map, buddy_idx)){ //==1
  //if buddy idx' is available, his children can't have bit set to 1...
    BitMap_setBit(&alloc->bit_map, idx, 0);
    BitMap_setBit(&alloc->bit_map, buddy_idx, 0);
    //2. we release the parent recursively
    BuddyAllocator_releaseBuddy(alloc, parent_idx);
  }
  else{
    BitMap_setBit(&alloc->bit_map, idx, 1);
  }
}

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, size_t size) {
  // we determine the level of the page
  int level = Buddyallocator_level(alloc, size+sizeof(int));
  printf("requested: %ld bytes, level %d \n", size, level);
  if(level==-1){
    printf("level = %d...too big size for buddy...\n", level);
    return NULL; //no suitable buddy found
  }
  // we get a buddy of that size;
  int idx = BuddyAllocator_getBuddy(alloc, level);
  printf("buddy allocator malloc---index value is:   %d\n", idx);
  if (idx==-1){
    printf("no memory available anymore\n");
    return NULL; //allocaton with buddy failed
  }
  printf("buddy at index %d assigned!\n", idx);
 //BitMap_setBit(&alloc->bit_map, idx, 0); //set the bit of the index
  // we write in the memory region managed the buddy address
  int buddyblock_size = alloc->mem_size / (1 << level); 	
  int start_index = startIdx(idx);
  char* buddyblock_start = alloc->memory + (start_index * buddyblock_size);
  *(int*)(buddyblock_start) = idx; 
  printf("memory allocated correctly\n\n");
  return buddyblock_start + sizeof(int); //memory address
}
//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem_addr) {
  if(!mem_addr) return;
  // we retrieve the buddy from the system
  //we need to calculate back the real index
  int idx = *((int*)mem_addr - sizeof(int));
  // sanity check
  assert(idx>=0);
  BuddyAllocator_releaseBuddy(alloc, idx);
  printf("memory deallocated correctly\n\n");
  return;
}
