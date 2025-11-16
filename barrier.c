/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here

// Simple global barrier structure
static struct {
  struct spinlock lock;  // protects the fields below
  int n;                 // number of processes that must reach the barrier
  int arrived;           // how many have arrived so far
} barrier;



int
barrier_init(int n)
{
  // initialize the lock and barrier counters
  initlock(&barrier.lock, "barrier");
  acquire(&barrier.lock);
  barrier.n = n;
  barrier.arrived = 0;
  release(&barrier.lock); 
  return 0;
}

int
barrier_check(void)
{
  acquire(&barrier.lock);

  barrier.arrived++;

  if (barrier.arrived < barrier.n) {
    // Not all processes have arrived yet.
    // Sleep on &barrier as the wait channel.
    sleep(&barrier, &barrier.lock);

    // When woken up, we hold barrier.lock again.
    release(&barrier.lock);
    return 0;
  } else {
    // This is the N-th process.
    // Wake up all processes sleeping on this barrier.
    wakeup(&barrier);

    // This one does not need to sleep.
    release(&barrier.lock);   
    return 0;
  }
}

/*----------xv6 sync lock end----------*/
