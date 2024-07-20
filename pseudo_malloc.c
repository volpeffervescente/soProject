#include <stddef.h>
/*for small requests (< 1/4 of the page size) it uses a
buddy allocator. Clearly, such a buddy allocator can
manage at most page-size bytes For simplicity use a 
single buddy allocator, implemented with a bitmap 
that manages 1 MB of memory for these small allocations.

for large request (>=1/4 of the page size) uses a mmap.*/

void* pseudo_malloc(size_t size);

void pseudo_free(void* mem_ptr);