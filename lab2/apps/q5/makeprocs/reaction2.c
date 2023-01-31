#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "spawn.h"

void main (int argc, char *argv[])
{
  sem_t s_procs_completed; // Semaphore to signal the original process that we're done

  if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
  } 
  
  // Convert the command-line strings into integers for use as handles
  sem_t h2o = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  sem_t h2 = dstrtol(argv[2], NULL, 10); // The "10" means base 10
  sem_t o2 = dstrtol(argv[3], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[4], NULL, 10);

  for(int i = 0; i < argv[5]; i++) {
    sem_wait(h2o);
    sem_wait(h2o);
    sem_signal(h2);
    sem_signal(h2);
    sem_signal(o2);
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("spawn_me: PID %d is complete.\n", Getpid());
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}
