Write a multi-thread program with following requirements:
- There are 4 worker threads and 1 primary thread
- Primary Thread's requirements:
  - Creates shared memory object enough to hold 115 64bits words for each thread. 
  - Spawns 4 worker threads, assigns a unique thread ID to each thread.
  - Let the worker threads know where can they write data to in the shared memory.
  - Worker threads form sort of linked list pattern with respect to the shared memory buffer. 
  - Ctrl+C should gracefully kill primary and worker threads. Print success and failure counts for each thread.

- Worker Threads' requirements:
  - Need to understand/know which thread is on the left and which is on the right (from shared memory prospective). 
  - Write the data (115 words of size 64bits) in the buffer area allocated for the worker thread on right. The value of data starts from a number and data increaments per 64bit word. 
  - Inform the worker thread on the right about the data it has written so that worker thread on the right can verify some sample of data value.
  - Verify some sample value (if not full buffer of size 115 64bits words) once informed by the thread on the left that it has written data to the buffer allocated to this thread. 
  - Maintain per thread counters for success and failure both. In case of data verification failure, don't abort, just update the failure counter and continue to the next sequence of data.  (e.g., increment failure count by one per sample of 115 words for any number of failures in that sample of data)
  - Threads should not be blocking for data availability from the left threads to send the data to the rigth thread.
  - Ctrl+C should gracefully kill all the threads. Perform above operation until then. 
  
CheckList:
- Create github account and update the code there (Talk with Monark and Rajesh for this, They will help you create github account such that it can be used for Bose also once we've your access setup)
- C++11 threads and callbacks
- Sparse the source/header files as needed. 
- Use CMake to compile the files
- Write UnitTest to test the requirements
- Should not be any memory leak

