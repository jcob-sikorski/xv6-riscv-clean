// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

int counter = 0;

struct run {
  struct run *next;
  int index;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct run allocpgs;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE) {
    kfree(p);
  }
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  r->index = counter++;
  kmem.freelist = r; // make r first element of freelist
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r, *temp;

  acquire(&kmem.lock);
  r = kmem.freelist; // get first element of freelist
  if(r) { // if first element exists
    kmem.freelist = r->next; // point freelist to its second element
    temp = kmem.freelist;
    if (temp)
      kmem.freelist = temp->next;
  }
  release(&kmem.lock);

  if (r) {
    if (temp) {
      r->next = temp;
      temp->next = &allocpgs;
    }
    else {
      r->next = &allocpgs;
    }
    allocpgs = *r; // make r first element of allocpgs
  }

  if(r) {
    memset((char*)r, 5, PGSIZE); // fill with junk
    if (temp)
      memset((char*)temp, 5, PGSIZE);
  }
  return (void*)r;
}

int
dump_allocated(int *frames, int numframes)
{
  if (numframes > counter) {
    return -1;
  }
  struct run *temp = &allocpgs;
  for (int i = 0; i < numframes; i++) {
    *frames = temp->index;
    frames++;
    temp = temp->next;
  }
  return 0;
}