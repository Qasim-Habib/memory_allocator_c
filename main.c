#include <stdlib.h>
#include "mem_allocator.h"
int main() {
    int* a=malloc(100*sizeof(int));
    void* temp2;
    void* temp;
    void* temp1;
    size_t max;
    MemoryAllocator* mem_alloc=MemoryAllocator_init(a,100*sizeof(int));
     temp=MemoryAllocator_allocate(mem_alloc,5);
     temp2=MemoryAllocator_allocate(mem_alloc,20);
    temp=MemoryAllocator_allocate(mem_alloc,13);
    MemoryAllocator_free(mem_alloc,temp2);
    temp2=MemoryAllocator_allocate(mem_alloc,7);
    temp1=MemoryAllocator_allocate(mem_alloc,5);
    temp=MemoryAllocator_allocate(mem_alloc,40);
    MemoryAllocator_free(mem_alloc,temp2);
    MemoryAllocator_free(mem_alloc,temp1);
    max=MemoryAllocator_optimize(mem_alloc);
    printf("%lu\n",max);
    temp=MemoryAllocator_allocate(mem_alloc,20);
    MemoryAllocator_free(mem_alloc,temp);
    temp=MemoryAllocator_release(mem_alloc);

    return 0;
}
