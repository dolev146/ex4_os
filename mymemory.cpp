// // #include <stdlib.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <math.h>
// #include <pthread.h>
// #include "mymemory.hpp"
// namespace mymemo
// {
//   // --- Global variables
//   chunkStatus *head = NULL;
//   chunkStatus *lastVisited = NULL;
//   pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//   void *brkPoint0 = NULL;

//   /* findChunk: search for the first chunk that fits (size equal or more) the request
//                 of the user.
//        chunkStatus *headptr: pointer to the first block of memory in the heap
//        unsigned int size: size requested by the user
//        retval: a poiter to the block which fits the request
//          or NULL, in case there is no such block in the list
//   */
//   chunkStatus *findChunk(chunkStatus *headptr, unsigned int size)
//   {
//     chunkStatus *ptr = headptr;

//     while (ptr != NULL)
//     {
//       if (ptr->size >= (size + STRUCT_SIZE) && ptr->available == 1)
//       {
//         return ptr;
//       }
//       lastVisited = ptr;
//       ptr = ptr->next;
//     }
//     return ptr;
//   }

//   /* splitChunk: split one big block into two. The first will have the size requested by the user.
//              the second will have the remainder.
//        chunkStatus* ptr: pointer to the block of memory which is going to be splitted.
//        unsigned int size: size requested by the user
//        retval: void, the function modifies the list
//   */
//   void splitChunk(chunkStatus *ptr, unsigned int size)
//   {
//     chunkStatus *newChunk;

//     newChunk = (chunkStatus *)ptr->end + size;
//     newChunk->size = ptr->size - size - STRUCT_SIZE;
//     newChunk->available = 1;
//     newChunk->next = ptr->next;
//     newChunk->prev = ptr;

//     if ((newChunk->next) != NULL)
//     {
//       (newChunk->next)->prev = newChunk;
//     }

//     ptr->size = size;
//     ptr->available = 0;
//     ptr->next = newChunk;
//   }

//   /* inscreaseAllocation: increase the amount of memory available in the heap, chaging its breakpoint
//        chunkStatus* ptr: pointer to the block of memory which is going to be splitted.
//        unsigned int size: size requested by the user
//        retval: void, the function modifies the list
//   */
//   chunkStatus *increaseAllocation(chunkStatus *lastVisitedPtr, unsigned int size)
//   {
//     brkPoint0 = sbrk(0);
//     chunkStatus *curBreak = (chunkStatus *)brkPoint0; // Current breakpoint of the heap

//     if (sbrk(MULTIPLIER * (size + STRUCT_SIZE)) == (void *)-1)
//     {
//       return NULL;
//     }

//     curBreak->size = (MULTIPLIER * (size + STRUCT_SIZE)) - STRUCT_SIZE;
//     curBreak->available = 0;
//     curBreak->next = NULL;
//     curBreak->prev = lastVisitedPtr;
//     lastVisitedPtr->next = curBreak;

//     if (curBreak->size > size)
//       splitChunk(curBreak, size);

//     return curBreak;
//   }

//   /* mergeChunkPrev: merge one freed chunk with its predecessor (in case it is free as well)
//        chunkStatus* freed: pointer to the block of memory to be freed.
//        retval: void, the function modifies the list
//   */
//   void mergeChunkPrev(chunkStatus *freed)
//   {
//     chunkStatus *prev;
//     prev = freed->prev;

//     if (prev != NULL && prev->available == 1)
//     {
//       prev->size = prev->size + freed->size + STRUCT_SIZE;
//       prev->next = freed->next;
//       if ((freed->next) != NULL)
//         (freed->next)->prev = prev;
//     }
//   }

//   /* mergeChunkNext: merge one freed chunk with the following chunk (in case it is free as well)
//        chunkStatus* freed: pointer to the block of memory to be freed.
//        retval: void, the function modifies the list
//   */
//   void mergeChunkNext(chunkStatus *freed)
//   {
//     chunkStatus *next;
//     next = freed->next;

//     if (next != NULL && next->available == 1)
//     {
//       freed->size = freed->size + STRUCT_SIZE + next->size;
//       freed->next = next->next;
//       if ((next->next) != NULL)
//         (next->next)->prev = freed;
//     }
//   }

//   /* printList: print the entire liked list. For debug purposes
//        chunkStatus* headptr: points to the begin of the list
//        retval: void, just print
//   */
//   // void printList(chunkStatus *headptr)
//   // {
//   //   int i = 0;
//   //   chunkStatus *p = headptr;

//   //   while (p != NULL)
//   //   {
//   //     printf("[%d] p: %d\n", i, p);
//   //     printf("[%d] p->size: %d\n", i, p->size);
//   //     printf("[%d] p->available: %d\n", i, p->available);
//   //     printf("[%d] p->prev: %d\n", i, p->prev);
//   //     printf("[%d] p->next: %d\n", i, p->next);
//   //     printf("__________________________________________________\n");
//   //     i++;
//   //     p = p->next;
//   //   }
//   // }

//   /* mymalloc: allocates memory on the heap of the requested size. The block
//                of memory returned should always be padded so that it begins
//                and ends on a word boundary.
//        unsigned int size: the number of bytes to allocate.
//        retval: a pointer to the block of memory allocated or NULL if the
//                memory could not be allocated.
//                (NOTE: the system also sets errno, but we are not the system,
//                       so you are not required to do so.)
//   */
//   void *mymalloc(unsigned int _size)
//   {
//     // pthread_mutex_lock(&lock);

//     void *brkPoint1;
//     unsigned int size = ALIGN(_size);
//     int memoryNeed = MULTIPLIER * (size + STRUCT_SIZE);
//     chunkStatus *ptr;
//     chunkStatus *newChunk;

//     pthread_mutex_lock(&lock);
//     brkPoint0 = sbrk(0);

//     if (head == NULL) // First time running: create free list
//     {
//       if (sbrk(memoryNeed) == (void *)-1) // error check
//       {
//         pthread_mutex_unlock(&lock);
//         return NULL;
//       }

//       // Create the first chunk with size equals all memory available in the heap after setting the new breakpoint
//       brkPoint1 = sbrk(0);
//       head = (chunkStatus *)brkPoint0;
//       head->size = memoryNeed - STRUCT_SIZE;
//       head->available = 0;
//       head->next = NULL;
//       head->prev = NULL;

//       // Split the chunk into two: one with size request by user, other with the remainder.
//       ptr = head;

//       // Verify if the split in the first allocation is necessary
//       if (MULTIPLIER > 1)
//         splitChunk(ptr, size);

//       pthread_mutex_unlock(&lock);

//       return ptr->end;
//     }

//     else // Not first time running
//     {
//       chunkStatus *freeChunk = NULL;
//       freeChunk = findChunk(head, size);

//       if (freeChunk == NULL) // Didn't find any chunk available
//       {
//         freeChunk = increaseAllocation(lastVisited, size); // Extend the heap
//         if (freeChunk == NULL)                             // Couldn't extend heap. increaseAllocation returned NULL (sbrk error)
//         {
//           pthread_mutex_unlock(&lock);
//           return NULL;
//         }
//         pthread_mutex_unlock(&lock);
//         return freeChunk->end;
//       }

//       else // A chunk was found
//       {
//         if (freeChunk->size > size) // If chunk is too big, split it
//           splitChunk(freeChunk, size);
//       }
//       pthread_mutex_unlock(&lock);
//       return freeChunk->end;
//     }
//   }

//   /* myfree: unallocates memory that has been allocated with mymalloc.
//        void *ptr: pointer to the first byte of a block of memory allocated by
//                   mymalloc.
//        retval: 0 if the memory was successfully freed and 1 otherwise.
//                (NOTE: the system version of free returns no error.)
//   */
//   unsigned int myfree(void *ptr)
//   {
//     //#if SYSTEM_MALLOC
//     /*free(ptr);
//     return 0;
//     #endif
//     return 1; */
//     pthread_mutex_lock(&lock);

//     chunkStatus *toFree;
//     toFree = (chunkStatus *)ptr - STRUCT_SIZE;

//     if (toFree >= head && toFree <= brkPoint0)
//     {
//       toFree->available = 1;
//       mergeChunkNext(toFree);
//       mergeChunkPrev(toFree);
//       pthread_mutex_unlock(&lock);
//       return 0;
//     }
//     else
//     {
//       //#endif
//       pthread_mutex_unlock(&lock);
//       return 1;
//     }
//   }
// }

#include <unistd.h>
#include <stddef.h>
#include "memory.h"

#define MIN_ALLOC(x) (x >= sizeof(struct m_block) ? sizeof(size_t) + x : sizeof(struct m_block))

// align  to 4 bytes (((x-1) / 4) * 4) + 4
#define ALIGN(x) ((((x - 1) >> 2) << 2) + 4)

typedef struct m_block m_block;

struct m_block
{
  size_t size;
  struct m_block *next;
};

m_block mem_list; // head of the list

void split_block(m_block *prev, size_t size)
{
  m_block *curr, *leftover;
  curr = prev->next;

  if (curr->size >= size + sizeof(m_block))
  {
    // split block
    leftover = (m_block *)((char *)curr + size);
    prev->next = leftover;
    leftover->next = curr->next;
    leftover->size = curr->size - size;
    curr->size = size;
  }
  else
  {
    prev->next = curr->next;
  }
}

m_block *extend_heap(m_block *prev, size_t size)
{
  void *p, *n;
  m_block *curr;

  p = sbrk(0); // get current breakpoint
  // if sbreak fails return null
  if (sbrk(size) == (void *)-1)
    return NULL;
  n = sbrk(0); // get the new breakpoint

  curr = (m_block *)p;
  curr->size = (char *)n - (char *)p; // set size of new block
  prev->next = curr;

  return curr;
}

void *malloc(size_t size)
{
  size_t size_al = ALIGN(size); // align to 4 bytes
  m_block *prev, *curr; // previous and current block

  size_al = MIN_ALLOC(size_al); // minimum allocation size

  prev = &mem_list; // start at head of list
  curr = mem_list.next; // start at next block
  while (curr)
  {
    if (curr->size >= size_al)
    {
      split_block(prev, size_al); // split block if necessary
      break;
    }

    prev = curr;
    curr = curr->next;
  }

  if (!curr)
  {
    curr = extend_heap(prev, size_al); // extend heap 
    if (!curr)
      return NULL;
    split_block(prev, size_al);
  }
  curr->next = NULL;
  return &(curr->next);
}


void *calloc(size_t nitems, size_t size)
{
  void *ptr = malloc(nitems * size);
  memset(ptr, 0, nitems * size);
  return ptr;
}

void free(void *ptr)
{
  if (ptr == NULL)
    return;

  m_block *blk = (m_block *)((char *)ptr - sizeof(size_t)); 
  m_block *prev, *next;
  size_t top;

  blk->next = NULL;
  prev = &mem_list;
  next = mem_list.next;

  while (next && next < blk)
  {
    prev = next;
    next = next->next;
  }

  if (prev == &mem_list)
  {
    top = 0;
  }
  else
  {
    top = (size_t)prev + prev->size;
  }

  if (top == (size_t)blk)
  {
    /* Coalesce with previous block */
    prev->size += blk->size;
    blk = prev;
  }
  else
  {
    blk->next = next;
    prev->next = blk;
  }

  if ((size_t)blk + blk->size == (size_t)next)
  {
    /* Coalesce with next block */
    blk->size += next->size; // add size of next block
    blk->next = next->next; // remove next block
  }
}