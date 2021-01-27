

#ifndef MEMALLOCPROJ_MEM_ALLOCATOR_H
#define MEMALLOCPROJ_MEM_ALLOCATOR_H
#include <stdio.h>
typedef struct MemoryAllocator MemoryAllocator;


MemoryAllocator* MemoryAllocator_init(void* memoryPool,size_t size);
void* MemoryAllocator_allocate(MemoryAllocator* allocator,size_t size);
void MemoryAllocator_free(MemoryAllocator* allocator, void*ptr);
size_t MemoryAllocator_optimize(MemoryAllocator* allocator);
void* MemoryAllocator_release(MemoryAllocator* allocator);
void merge_free_blocks(size_t cur_count,void* m_ptr,MemoryAllocator* allocator);
#endif 
