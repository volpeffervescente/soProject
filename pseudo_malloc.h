#pragma once
#include "bit_map.h"
#include "buddy_allocator.h"

#define PAGE_SIZE 4096
#define BUDDY_ALLOCATOR_THRESHOLD (PAGE_SIZE / 4)
#define BUDDY_ALLOCATOR_SIZE (1024 * 1024) // 1 MB
#define MAX_LEVELS 9
#define MIN_BUDDY_SIZE (BUDDY_ALLOCATOR_SIZE >> (MAX_LEVELS - 1))

void pseudo_malloc_init(); //allocator initialization

void* pseudo_malloc(size_t size); //for actual memory allocation

void pseudo_free(void* mem_ptr, size_t size);  //for proper memory deallocation