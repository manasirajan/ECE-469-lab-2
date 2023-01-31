#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "spawn.h"

void main (int argc, char *argv[])
{
  int numprocs = 5;               // Used to store number of processes to create
  int i;                          // Loop index variable
  uint32 h_mem;                   // Used to hold handle to shared memory page
  sem_t s_procs_completed;        // Semaphore used to wait until all spawned processes have completed
  char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes

  run_os_tests(); // Use for testing purpose

  if (argc != 3) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of processes to create>\n");
    Exit();
  }

  //get number of NO molecules
  int num_no = dstrtol(argv[1], NULL, 10);

  //get number of H2O molecules 
  int num_h2o = dstrtol(argv[2], NULL, 10);


  // Allocate space for a shared memory page, which is exactly 64KB
  // Note that it doesn't matter how much memory we actually need: we 
  // always get 64KB
  /*
  if ((h_mem = shmget()) == 0) {
    Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  // Map shared memory page into this process's memory space
  if ((buff = (buffer *)shmat(h_mem)) == NULL) {
    Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
    Exit();
  }

  // Put some values in the shared memory, to be read by other processes
  buff->head = 0;
  buff->tail = 0;
  */

  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.  To do this, we will initialize
  // the semaphore to (-1) * (number of signals), where "number of signals"
  // should be equal to the number of processes we're spawning - 1.  Once 
  // each of the processes has signaled, the semaphore should be back to
  // zero and the final sem_wait below will return.
  if ((s_procs_completed = sem_create(-(numprocs-1))) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  } 

  sem_t no;
  sem_t n2;
  sem_t o2;
  sem_t h2o;
  sem_t h2;
  sem_t hno3;
  if ((no = sem_create(0) == SYNC_FAIL)) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();  	
  }
  if ((n2 = sem_create(0) == SYNC_FAIL)) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();  	
  }
  if ((o2 = sem_create(0) == SYNC_FAIL)) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();  	
  }
  if ((h2o = sem_create(0) == SYNC_FAIL)) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();  	
  }
  if ((h2 = sem_create(0) == SYNC_FAIL)) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();  	
  }
  if ((hno3 = sem_create(0) == SYNC_FAIL)) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();  	
  }



  // Setup the command-line arguments for the new process.  We're going to
  // pass the handles to the shared memory page and the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  //ditoa(h_mem, h_mem_str);
  ditoa(s_procs_completed, s_procs_completed_str);
  ditoa(no, no_completed_str);
  ditoa(n2, n2_completed_str);
  ditoa(o2, o2_completed_str);
  ditoa(h2o, h2o_completed_str);
  ditoa(h2, h2_completed_str);
  ditoa(hno3, hno3_completed_str);
  
  // ditoa(mutex, mutex_str);
  // Now we can create the processes.  Note that you MUST end your call to
  // process_create with a NULL argument so that the operating system
  // knows how many arguments you are sending.
  for(i=0; i<numprocs; i++) {
    process_create(INJECTION1, no_completed_str, s_procs_completed_str, argv[1], NULL);
    process_create(INJECTION2, h2o_completed_str, s_procs_completed_str, argv[2], NULL);
    process_create(PROCESS1, no_completed_str, n2_completed_str, o2_completed_str, s_procs_completed_str, argv[1]/2, NULL);
    process_create(PROCESS2, h2o_completed_str, h2_completed_str, o2_completed_str, s_procs_completed_str, argv[2]/2, NULL);
    process_create(PROCESS3, h2_completed_str, n2_completed_str, o2_completed_str, hno3_completed_str, s_procs_completed_str, (argv[1] + argv[2])/6, NULL);
    Printf("Process %d created\n", i);
  }

  // And finally, wait until all spawned processes have finished.
  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("All other processes completed, exiting main process.\n");
}
