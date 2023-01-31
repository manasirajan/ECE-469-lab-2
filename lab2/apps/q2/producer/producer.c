#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "spawn.h"

void main (int argc, char *argv[])
{
  buffer *buff;        // Used to access missile codes in shared memory page
  uint32 h_mem;            // Handle to the shared memory page
  sem_t s_procs_completed; // Semaphore to signal the original process that we're done
  lock_t mutex;
  char * helloworld = "Hello World";

  if (argc != 4) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
  } 

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[2], NULL, 10);
  mutex = dstrol(argv[2], NULL, 10);

  // Map shared memory page into this process's memory space
  if ((buff = (buffer *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
 
  // Now print a message to show that everything worked
  Printf("buffer head %d \n.  ", buff->head);
  Printf("buffer tail %d\n", buff->tail);
  Printf("spawn_me: My PID is %d\n", Getpid());


//producer code
for (int i=0; i<dstrlen(helloworld); i++){
int written = 0;

while(!written){
    if (lock_acquire(mutex)==SYNC_FAIL){
        Printf("Lock acquire fail in");Printf(argv[0]);Printf("exiting... \n");
        Exit();
    }
    if ((buff->head +1)%BUFFSIZE != buff->tail){
        buff->content[head] = helloworld[i];
        buff->head = (buff->head + 1)%BUFFSIZE;
        written  = 1;
        Printf("Producer %d", Getpid()); Printf("inserted: %c\n", helloworld[i]);

    }
    if (lock_release(mutex)==SYNC_FAIL){
        Printf("Lock release fail in");Printf(argv[0]);Printf("exiting... \n");
        Exit();
    }

}


}

  // Signal the semaphore to tell the original process that we're done
  Printf("spawn_me: PID %d is complete.\n", Getpid());
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}
