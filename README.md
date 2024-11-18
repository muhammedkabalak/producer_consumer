# producer_consumer
Implementation of the Producer-Consumer problem using C. The program demonstrates synchronization between producer and consumer threads, ensuring proper resource management.

**Objective:**
The goal of this project is to implement the classic producer-consumer problem using threads, semaphores, and mutexes. A producer thread generates random odd numbers and places them into a shared circular buffer, while a consumer thread retrieves and processes the numbers.

**Implementation:**
1. **Circular Buffer**: A fixed-size buffer of size `BUFFER_SIZE` (10 in this case) is used to store the produced items. The buffer uses two pointers: `in` for the producer's insertion point and `out` for the consumer's removal point.
   
2. **Synchronization**:
   - **Semaphores**: 
     - `empty` controls the number of available empty slots in the buffer. It starts at `BUFFER_SIZE` (full capacity).
     - `full` controls the number of items in the buffer. It starts at 0 (empty).
   - **Mutex**: Ensures that only one thread (either producer or consumer) can access the buffer at a time to avoid race conditions.

3. **Producer**:
   - Generates a random odd number between 31 and 10001.
   - Adds it to the buffer if there is space (controlled by the `empty` semaphore).
   - After placing an item, signals the consumer via the `full` semaphore.

4. **Consumer**:
   - Retrieves and processes an item from the buffer if it’s not empty (controlled by the `full` semaphore).
   - After consuming an item, signals the producer via the `empty` semaphore.

5. **Thread Synchronization**:
   - The producer and consumer run in infinite loops, producing and consuming items asynchronously.
   - `pthread_join()` ensures that the main thread waits for the producer and consumer threads to complete (though they run indefinitely).

**Key Functions**:
- `sem_wait()` and `sem_post()` are used to block or signal the semaphores, ensuring proper synchronization.
- `pthread_mutex_lock()` and `pthread_mutex_unlock()` ensure that only one thread modifies the buffer at a time.

**Output**:
The program prints the produced and consumed numbers along with the current buffer state, showing how the producer and consumer interact while maintaining synchronization.

**Conclusion**:
This implementation successfully simulates the producer-consumer problem using threads, semaphores, and mutexes. It demonstrates how synchronization techniques can be used to manage shared resources in a multithreaded environment.

