#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_getfilenum(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  
  return getfilenum(pid);
}

uint sys_mprotect(void)
{
  uint64 addr;
  uint64 len;

  if(argaddr(0, &addr) < 0)
    return -1;
  if(argaddr(1, &len) < 0)
    return -1;
  
  // checking len bounds
  if(len == 0 || len < 0 || len > myproc()->sz)
    return -1;
  // checking addr bounds
  if(addr < 0 || addr == KERNBASE || addr > KERNBASE)
    return -1;
	// checking addr alignmnet
  if(((unsigned long)addr & 15) != 0)
    return -1;

  pagetable_t pagetable = myproc()->pagetable;

  // get page table entry
  for(int level = 2; level > 0; level--) {
    pte_t *pte = &(pagetable)[PX(level, addr)];
    pagetable = (pagetable_t)PTE2PA(*pte);
  }

  // change protection bits for "len" pages
  for(int i = 0; i < len; i++) {
    pte_t *pte = &pagetable[PX(0, addr + i)];
    *pte &= ~PTE_W;
  }
  
  // tell the hardware that the page table has changed
  sfence_vma();

  return 0;
}

uint sys_munprotect(void)
{
  uint64 addr;
  uint64 len;

  if(argaddr(0, &addr) < 0)
    return -1;
  if(argaddr(1, &len) < 0)
    return -1;
  
  // checking len bounds
  if(len == 0 || len < 0 || len > myproc()->sz)
    return -1;
  // checking addr bounds
  if(addr < 0 || addr == KERNBASE || addr > KERNBASE)
    return -1;
	// checking addr alignmnet
  if(((unsigned long)addr & 15) != 0)
    return -1;

  pagetable_t pagetable = myproc()->pagetable;

  // get page table entry
  for(int level = 2; level > 0; level--) {
    pte_t *pte = &(pagetable)[PX(level, addr)];
    pagetable = (pagetable_t)PTE2PA(*pte);
  }

  // change protection bits for "len" pages
  for(int i = 0; i < len; i++) {
    pte_t *pte = &pagetable[PX(0, addr + i)];
    *pte |= PTE_W;
  }
  
  // tell the hardware that the page table has changed
  sfence_vma();

  return 0;
}

uint sys_dump_allocated(void) 
{
  uint64 frames_ptr;
  int numframes;
  if(argaddr(0, (uint64*) &frames_ptr) < 0)
    return -1;
  if(argint(1, &numframes) < 0)
    return -1;

  int frames[numframes];

  copyin(myproc()->pagetable, (uint64) &frames, frames_ptr, (uint64) sizeof(frames));
  dump_allocated(frames, numframes);
  copyout(myproc()->pagetable, frames_ptr, (char*) &frames, (uint64) sizeof(frames));
  return 0;
}