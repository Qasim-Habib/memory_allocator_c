

#include "mem_allocator.h"
#define padding 16

struct MemoryAllocator{

    void* m_pool;
    size_t m_size;
};
MemoryAllocator allocator;
MemoryAllocator* MemoryAllocator_init(void* memoryPool,size_t size)
{
    if(size%padding!=0){
        size+=(padding-(size%padding));
    }
    allocator.m_pool=memoryPool;
    *(size_t*)allocator.m_pool=size;
    allocator.m_size=size;
    return &allocator;
}
void* MemoryAllocator_allocate(MemoryAllocator* allocator,size_t size)
{
    unsigned int check=0x01;
    size_t count=0;
    size_t size_of_block;
    size_t new_size;
   /* char* temp;*/
    char* cur_ptr=(char*)allocator->m_pool;
    if(size>allocator->m_size)
        return NULL;
    new_size=size+sizeof(size_t);
    if(new_size%padding==0)
        size=new_size;
    else
        size=(padding-(new_size%padding))+new_size;
    while(count<allocator->m_size) {
        size_of_block = *(size_t *) cur_ptr;
       
        if ((*(size_t *) cur_ptr) & check) {
            size_of_block ^= check;
            cur_ptr += (size_of_block);
            count += size_of_block;
            continue;
        }
        if (size == size_of_block) {
            *((size_t *) cur_ptr) = size;
            *(size_t *) cur_ptr |= check;
            cur_ptr += (sizeof(size_t));
            return cur_ptr;
        }
        if (size < size_of_block) {
            *((size_t *) cur_ptr) = size;
            *(size_t *) cur_ptr |= check;
            *((size_t *) (cur_ptr + size)) = size_of_block - size;
            cur_ptr += (sizeof(size_t));
            return cur_ptr;
        }
        merge_free_blocks(count,cur_ptr,allocator);
        if(size_of_block==*(size_t*)cur_ptr) {
            cur_ptr += (size_of_block-1);
            count+=size_of_block;
        }
    }
    return NULL;
}
void merge_free_blocks(size_t cur_count,void* m_ptr,MemoryAllocator* allocator)
{
    unsigned int check=0x01;
    char* temp=(char*)m_ptr;
    size_t size_of_block;
    size_t cur_size=0;
    while(cur_count<allocator->m_size)
    {
        size_of_block = *(size_t *) temp;
        if (size_of_block & check) {
            break;
        }
        cur_size+=size_of_block;
        cur_count+=size_of_block;
        *(size_t*)m_ptr=cur_size;
        temp+=size_of_block;
    }

}
void MemoryAllocator_free(MemoryAllocator* allocator, void*ptr)
{
    char* cur_ptr=(char*)allocator->m_pool;
    void* temp;
    unsigned int check=0x01;
    size_t size_of_block;
    size_t count=0;
    if(ptr!=NULL){
        while (count<allocator->m_size){
            size_of_block=*(size_t*)cur_ptr;
            temp=cur_ptr+sizeof(size_t);
            if(temp==ptr){
                if ((*(size_t *) cur_ptr) & check) {
                    *(size_t*)cur_ptr^=check;
                }
                return;
            }
            if ((*(size_t *) cur_ptr) & check) {
                size_of_block^=check;
            }
            cur_ptr+=size_of_block;
            count+=size_of_block;
        }

    }
}

void* MemoryAllocator_release(MemoryAllocator* allocator)
{
    char* temp=(char*)allocator->m_pool;
    *(size_t*)temp=allocator->m_size;
    return temp;
}

size_t MemoryAllocator_optimize(MemoryAllocator* allocator)
{
    size_t max=0;
    unsigned int check=0x01;
    char* cur_ptr=(char*)allocator->m_pool;
    void* temp;
    size_t size_of_block;
    size_t count=0;
    size_t cur_size=0;
    while (count<allocator->m_size) {
        size_of_block = *(size_t *)cur_ptr;
        if (size_of_block&check) {
            size_of_block ^= check;
            cur_ptr += size_of_block;
            count += size_of_block;
            continue;
        }
        cur_size+=size_of_block;
        temp=cur_ptr+size_of_block;
        count += size_of_block;
        while(count<allocator->m_size&&(*(size_t*)temp&check)==0){
            cur_size+=(*(size_t*)temp);
            *(size_t*)cur_ptr=cur_size;
            count +=(*(size_t*)temp) ;
            temp=(char*)temp+(*(size_t*)temp);
        }
        if(cur_size>max)
            max=cur_size;
        cur_size=0;
        cur_ptr+=(*(size_t*)cur_ptr);
    }
    return max;
}
